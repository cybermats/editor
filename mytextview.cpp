//
// Created by mats on 11/10/18.
//

#include "mytextview.h"
#include <cstring>
#include <iostream>

void MyTextView::set_formatter(const std::string& name, Formatter *formatter) {
  m_formatters[name] = formatter;
}


bool MyTextView::move_caret_left() {
  if (_caret.column > 0) {
    _caret.column--;
  } else {
    if (move_caret_up())
      _caret.state = PositionState::END;
  }
  return true;
}

bool MyTextView::move_caret_right() {
  if (_caret.column < _screen_line_sizes.at(_caret.line - m_display_top_line_position)) {
    _caret.column++;
  } else {
    if (move_caret_down())
      _caret.column = 0;
  }
  return true;
}

bool MyTextView::move_caret_up() {
  // The caret is above the current viewing window
  if (_caret.line < m_display_top_line_position) {
    m_update_display_top_line_position = _caret.line - 2;
  }
    // The caret is below the current viewing window
  else if (_caret.line > m_display_top_line_position + m_current_display_line_count) {
    auto top_line = _caret.line - m_current_display_line_count + 1;
    m_update_display_top_line_position = top_line;
  }
    // The caret is within the current viewing window
  else {
    if (_caret.line == m_display_top_line_position) {
      if (m_display_top_line_position > 0)
        m_update_display_top_line_position =
                m_display_top_line_position - 1;
      else
        m_update_display_top_line_position = 0;
    }
  }


  if (_caret.line == 0) {
    _caret.column = 0;
    return false;
  }
  _caret.line--;
  _caret.state = PositionState::UNKNOWN;
  return true;
}

bool MyTextView::move_caret_down() {
  // The caret is above the current viewing window
  if (_caret.line < m_display_top_line_position) {
    m_update_display_top_line_position = _caret.line;
  }
    // The caret is below the current viewing window
  else if (_caret.line > m_display_top_line_position + m_current_display_line_count) {
    auto top_line = _caret.line - m_current_display_line_count + 3;
    m_update_display_top_line_position = top_line;
  }
    // The caret is within the current viewing window
  else {
    if (_caret.line == m_display_top_line_position + m_current_display_line_count - 1) {
      m_update_display_top_line_position = m_display_top_line_position + 1;
    }
  }
  if (_caret.line == _buffer.line_count() - 1) {
    _caret.line = _buffer.line_count() - 1;
    _caret.state = PositionState::END;
    return false;
  }
  _caret.line++;
  _caret.state = PositionState::UNKNOWN;
  return true;
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
  m_update_display_top_line_position = line;
}

bool MyTextView::update_display_size(double display_line_count) {
  if ((((unsigned long)display_line_count) != m_current_display_line_count)
  || (m_display_top_line_position != m_update_display_top_line_position)
  || m_force_update)
  {
    _screen_lines.clear();
    _screen_line_sizes.clear();
    m_display_top_line_position = m_update_display_top_line_position;
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
    move_caret_left();
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

void MyTextView::set_selection_start_relative(unsigned long line, unsigned long column) {
  m_selectionStart.line = m_display_top_line_position + line;
  m_selectionStart.column = column;
  m_selectionStart.state = PositionState::UNKNOWN;
}

void MyTextView::set_selection_end_relative(unsigned long line, unsigned long column) {
  m_selectionEnd.line = m_display_top_line_position + line;
  m_selectionEnd.column = column;
  m_selectionEnd.state = PositionState::UNKNOWN;
}
