//
// Created by mats on 11/10/18.
//

#pragma once

#include <vector>
#include <map>
#include "mytextbuffer.h"
#include "formatter/formatter.h"
#include "position.h"

// TODO(mats): Handle the state consistency when rows have been deleted. It freaks out when scrolling to the bottom.

class MyTextView {

public:

  bool calculate_display_window();

  void move_caret_forward(const std::string& step_size);

  void move_caret_backward(const std::string& step_size);

  Position get_next_position(
      Position position, const std::string& step_size);

  Position get_previous_position(
      Position position, const std::string& step_size);

    void set_caret_relative(unsigned long line, unsigned long column);

    void set_selection_start_relative(unsigned long line, unsigned long column);

  Position get_selection_begin() const;

  Position get_selection_end() const;

    void set_selection_finish_relative(unsigned long line, unsigned long column);

    void set_file(const std::string &filename);

    void update_top_line(unsigned long line);

    bool update_display_size(double display_line_count);

    size_t size() const;

    size_t display_count() const;

    const char *get_line(unsigned long display_line);

    long get_caret_pos(unsigned long display_line);

  unsigned long get_actual_line(unsigned long display_line);

    unsigned long get_display_top_line_position() const;

    void insert(const char *data, unsigned long size);

    void remove(bool back);

private:
    MyTextBuffer _buffer;
    std::map<std::string, Formatter*> m_formatters;
    std::vector<std::vector<char>> _screen_lines;
    std::vector<unsigned long> _screen_line_sizes;

    Position _caret;

    Position m_selectionStart;
    Position m_selectionFinish;

    unsigned long m_current_display_line_count = 0;
    unsigned long m_display_top_line_position = 0;
    bool m_force_update = false;

};
