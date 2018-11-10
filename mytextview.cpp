//
// Created by mats on 11/10/18.
//

#include "mytextview.h"
#include <cstring>


bool MyTextView::move_caret_left() {
  if (_caret_column > 0) {
    _caret_column--;
  } else {
    if (move_caret_up())
      _caret_pos = caret_pos_t::END;
  }
  return true;
}

bool MyTextView::move_caret_right() {
  if (_caret_column < _screen_line_sizes.at(_caret_line - m_display_top_line_position)) {
    _caret_column++;
  } else {
    if (move_caret_down())
      _caret_column = 0;
  }
  return true;
}

bool MyTextView::move_caret_up() {
  // The caret is above the current viewing window
  if (_caret_line < m_display_top_line_position) {
    m_update_display_top_line_position = _caret_line - 2;
  }
    // The caret is below the current viewing window
  else if (_caret_line > m_display_top_line_position + m_current_display_line_count) {
    auto top_line = _caret_line - m_current_display_line_count + 1;
    m_update_display_top_line_position = top_line;
  }
    // The caret is within the current viewing window
  else {
    if (_caret_line == m_display_top_line_position) {
      if (m_display_top_line_position > 0)
        m_update_display_top_line_position =
                m_display_top_line_position - 1;
      else
        m_update_display_top_line_position = 0;
    }
  }


  if (_caret_line == 0) {
    _caret_column = 0;
    return false;
  }
  _caret_line--;
  return true;
}

bool MyTextView::move_caret_down() {
  // The caret is above the current viewing window
  if (_caret_line < m_display_top_line_position) {
    m_update_display_top_line_position = _caret_line;
  }
    // The caret is below the current viewing window
  else if (_caret_line > m_display_top_line_position + m_current_display_line_count) {
    auto top_line = _caret_line - m_current_display_line_count + 3;
    m_update_display_top_line_position = top_line;
  }
    // The caret is within the current viewing window
  else {
    if (_caret_line == m_display_top_line_position + m_current_display_line_count - 1) {
      m_update_display_top_line_position = m_display_top_line_position + 1;
    }
  }
  if (_caret_line == _buffer.line_count() - 1) {
    _caret_line = _buffer.line_count() - 1;
    _caret_pos = caret_pos_t::END;
    return false;
  }
  _caret_line++;
  return true;
}

void MyTextView::set_caret_relative(unsigned long line, unsigned long column) {
  _caret_line = m_display_top_line_position + line;
  _caret_column = column;
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
  if ((display_line + m_display_top_line_position) == _caret_line) {
    if (_caret_pos == caret_pos_t::END) {
      _caret_column = _screen_line_sizes[_caret_line - m_display_top_line_position];
    }
    _caret_pos = caret_pos_t::KNOWN;
    auto caret_column = std::min(_caret_column, _screen_line_sizes[_caret_line - m_display_top_line_position]);
    return caret_column;
  }
  else {
    return -1;
  }

}

unsigned long MyTextView::get_display_top_line_position() const {
  return m_display_top_line_position;
}

void MyTextView::insert(const char *data, unsigned long size) {
  _buffer.insert(_caret_line, _caret_column, data, size);
  _caret_column++;
  m_force_update = true;
}
