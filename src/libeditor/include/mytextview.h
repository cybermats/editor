//
// Created by mats on 11/10/18.
//

#pragma once

#include <vector>
#include <map>
#include "mytextbuffer.h"
#include "formatter/formatter.h"

// TODO(mats): Handle the state consistency when rows have been deleted. It freaks out when scrolling to the bottom.

class MyTextView {

public:

    void set_formatter(const std::string& name, Formatter* formatter);

    bool move_caret_up();

    bool move_caret_down();

    bool move_caret_left();

    bool move_caret_right();

    void set_caret_relative(unsigned long line, unsigned long column);

    void set_selection_start_relative(unsigned long line, unsigned long column);

    void set_selection_end_relative(unsigned long line, unsigned long column);

    void set_file(const std::string &filename);

    void update_top_line(unsigned long line);

    bool update_display_size(double display_line_count);

    size_t size() const;

    size_t display_count() const;

    const char *get_line(unsigned long display_line);

    long get_caret_pos(unsigned long display_line);

    unsigned long get_display_top_line_position() const;

    void insert(const char *data, unsigned long size);

    void remove(bool back);

private:
    enum class PositionState {
        KNOWN,
        UNKNOWN,
        END
    };

    struct Position {
        unsigned long line = 0;
        unsigned long column = 0;
        PositionState state = PositionState::KNOWN;
    };

    MyTextBuffer _buffer;
    std::map<std::string, Formatter*> m_formatters;
    std::vector<std::vector<char>> _screen_lines;
    std::vector<unsigned long> _screen_line_sizes;

    Position _caret;

    Position m_selectionStart;
    Position m_selectionEnd;

    unsigned long m_current_display_line_count = 0;
    unsigned long m_display_top_line_position = 0;
    unsigned long m_update_display_top_line_position = 0;
    bool m_force_update = false;

};


