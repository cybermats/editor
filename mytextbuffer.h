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

    const char* get_line(unsigned long line_num);
    unsigned long line_count() const {
      return _buffer.size();
    }

private:
    std::vector<std::vector<char>> _buffer;

};


