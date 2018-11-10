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

MyTextBuffer::MyTextBuffer() {

}

void MyTextBuffer::load(const std::string& filename) {
  std::ifstream input(filename);
  char buffer[1024];
  _buffer.clear();
  while(input.getline(buffer, 1024)) {
    auto length = strlen(buffer);
    _buffer.push_back(std::vector<char>(buffer, buffer + length + 1));
  }

}