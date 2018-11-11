//
// Created by mats on 11/10/18.
//

#pragma once

#include <vector>
#include <map>
#include "mytextbuffer.h"

class MyTextView {

public:

    bool move_caret_up();

    bool move_caret_down();

    bool move_caret_left();

    bool move_caret_right();

    void set_caret_relative(unsigned long line, unsigned long column);

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
    enum class caret_pos_t {
        KNOWN,
        UNKNOWN,
        END
    };

    MyTextBuffer _buffer;
    std::vector<std::vector<char>> _screen_lines;
    std::vector<unsigned long> _screen_line_sizes;

    unsigned long _caret_line = 0;
    unsigned long _caret_column = 0;
    caret_pos_t _caret_pos = caret_pos_t::KNOWN;

    unsigned long m_current_display_line_count = 0;
    unsigned long m_display_top_line_position = 0;
    unsigned long m_update_display_top_line_position = 0;
    bool m_force_update = false;

};


