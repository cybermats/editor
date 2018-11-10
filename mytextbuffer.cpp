//
// Created by mats on 2018-11-07.
//

#include "mytextbuffer.h"
#include <fstream>
#include <iostream>
#include <cstring>

const char* MyTextBuffer::get_line(unsigned long line_num) {
  return &_buffer[line_num][0];
}

MyTextBuffer::MyTextBuffer() = default;

void MyTextBuffer::load(const std::string& filename) {
  std::ifstream input(filename);
  char buffer[1024];
  _buffer.clear();
  while(input.getline(buffer, 1024)) {
    auto length = strlen(buffer);
    _buffer.emplace_back(buffer, buffer + length + 1);
  }

}

void MyTextBuffer::insert(unsigned long line_num, unsigned long column, const char *data, unsigned long size) {
  auto& line = _buffer[line_num];
  line.insert(line.begin() + column, data, data + size);
}
