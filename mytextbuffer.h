//
// Created by mats on 2018-11-07.
//

#pragma once

#include <vector>
#include <string>

class MyTextBuffer {
public:
    MyTextBuffer();

    void load(const std::string& filename);

    void insert(unsigned long line_num, unsigned long column, const char* data, unsigned long size);
    void remove(unsigned long line_num, unsigned long column, bool back);

    const char* get_line(unsigned long line_num);
    unsigned long line_count() const {
      return _buffer.size();
    }

private:
    std::vector<std::vector<char>> _buffer;

};


