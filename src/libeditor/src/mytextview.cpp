//
// Created by mats on 11/10/18.
//

#include "mytextview.h"
#include <cstring>
#include <iostream>

Position MyTextView::get_selection_begin() const {
  if (m_selectionStart < m_selectionFinish)
    return m_selectionStart;
  else
    return m_selectionFinish;
}

Position MyTextView::get_selection_end() const {
  if (m_selectionStart < m_selectionFinish)
    return m_selectionFinish;
  else
    return m_selectionStart;
}

Position MyTextView::get_next_position(
    Position position, const std::string& step_size) {
  auto str = _buffer.get_line(position.line);

  if (step_size == "character") {
    if (position.column < strlen(str)) {
      position.column++;
    }
    else {
      position.column = 0;
      if (position.line < _buffer.line_count() - 1)
        position.line++;
    }
    position.state = PositionState::KNOWN;
  }
  else if (step_size == "line") {
    if (position.line < _buffer.line_count() - 1) {
      position.line++;
      position.state = PositionState::UNKNOWN;
    }
  }
  return position;
}

Position MyTextView::get_previous_position(
    Position position, const std::string& step_size) {

  if (step_size == "character") {
    if (position.column > 0) {
      position.column--;
      position.state = PositionState::KNOWN;
    }
    else {
      if (position.line > 0) {
        position.line--;
        position.state = PositionState::END;
      }
    }
  }
  else if (step_size == "line") {
    if (position.line > 0) {
      position.line--;
      position.state = PositionState::UNKNOWN;
    }
  }

  return position;
}

void MyTextView::move_caret_forward(const std::string& step_size) {
  _caret = get_next_position(_caret, step_size);
  calculate_display_window();
}

void MyTextView::move_caret_backward(const std::string& step_size) {
  _caret = get_previous_position(_caret, step_size);
  calculate_display_window();
}

unsigned long MyTextView::get_actual_line(unsigned long display_line) {
  return m_display_top_line_position + display_line;
}

bool MyTextView::calculate_display_window() {
  if (_caret.line < m_display_top_line_position) {
    m_display_top_line_position = _caret.line;
    m_force_update = true;
    return true;
  }
  else if (_caret.line >= m_display_top_line_position +
           m_current_display_line_count) {
    m_display_top_line_position =
        _caret.line - m_current_display_line_count + 1;
    m_force_update = true;
    return true;
  }
  return false;
}

void MyTextView::set_caret_relative(unsigned long line, unsigned long column) {
  _caret.line = m_display_top_line_position + line;
  _caret.column = column;
  _caret.state = PositionState::UNKNOWN;
}

void MyTextView::set_file(const std::string &filename) {
  _buffer.load(filename);
}

size_t MyTextView::size() const {
  return _buffer.line_count();
}

void MyTextView::update_top_line(unsigned long line) {
  m_display_top_line_position = line;
  m_force_update = true;
}

bool MyTextView::update_display_size(double display_line_count) {
  if ((((unsigned long)display_line_count) != m_current_display_line_count)
      || m_force_update)
  {
    _screen_lines.clear();
    _screen_line_sizes.clear();
    m_current_display_line_count = (unsigned long)display_line_count;
    bool fraction = (display_line_count - m_current_display_line_count) > 0.1;
    if (m_current_display_line_count + m_display_top_line_position >= _buffer.line_count()) {
      m_current_display_line_count = _buffer.line_count() - m_display_top_line_position;
      fraction = false;
    }

    auto range = m_current_display_line_count;
    if (fraction)
      range++;

    for (unsigned long i = 0; i < range; ++i) {
      auto line_no = m_display_top_line_position + i;
      auto line = _buffer.get_line(line_no);
      auto length = strlen(line);
      _screen_lines.emplace_back(line, line + length + 1);
      _screen_line_sizes.push_back(length);
    }
    m_force_update = false;

    return true;
  }
  return false;
}

const char *MyTextView::get_line(unsigned long display_line) {
  return &_screen_lines[display_line][0];
}

long MyTextView::get_caret_pos(unsigned long display_line) {
  if ((display_line + m_display_top_line_position) == _caret.line) {
    if (_caret.state == PositionState::END) {
      _caret.column = _screen_line_sizes[_caret.line - m_display_top_line_position];
    }
    else if (_caret.state == PositionState::UNKNOWN) {
      _caret.column = std::min(_caret.column, _screen_line_sizes[_caret.line - m_display_top_line_position]);

    }
    _caret.state = PositionState::KNOWN;
    return _caret.column;
  }
  else {
    return -1;
  }

}

unsigned long MyTextView::get_display_top_line_position() const {
  return m_display_top_line_position;
}

void MyTextView::insert(const char *data, unsigned long size) {
  _buffer.insert(_caret.line, _caret.column, data, size);
  _caret.column++;
  m_force_update = true;
}

void MyTextView::remove(bool back) {
  auto line = _caret.line;
  auto col = _caret.column;

  if (col == 0) {

  }

  if (back) {
    move_caret_forward("character");
    if (col == 0 && line > 0) {
      auto prev_data = _buffer.get_line(line - 1);
      auto prev_size = strlen(prev_data);
      _caret.column = prev_size;
      _caret.state = PositionState::KNOWN;
      std::cout << "Fixed caret" << std::endl;
    }
  }

  _buffer.remove(line, col, back);
  m_force_update = true;
}

size_t MyTextView::display_count() const {
  return m_current_display_line_count;
}

void MyTextView::set_selection_start_relative(
    unsigned long line, unsigned long column) {
  auto begin = m_display_top_line_position + line;
  m_selectionStart.line = begin;
  m_selectionStart.column = column;
  m_selectionStart.state = PositionState::UNKNOWN;

  m_selectionFinish = m_selectionStart;
}

void MyTextView::set_selection_finish_relative(
    unsigned long line, unsigned long column) {
  auto begin = m_display_top_line_position + line;

  m_selectionFinish.line = begin;
  m_selectionFinish.column = column;
  m_selectionFinish.state = PositionState::UNKNOWN;
}
