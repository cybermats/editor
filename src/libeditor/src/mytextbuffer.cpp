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

void MyTextBuffer::remove(unsigned long line_num, unsigned long column, bool back) {
  // Check that we're not in the beginning and tries to backspace
  if (back && line_num == 0 && column == 0)
    return;
  // Or that we're at the end and try to delete.
  if (!back && line_num == (_buffer.size()-1) && column == _buffer.back().size())
    return;

  // Convert the backspace into a delete
  if (back && column == 0) {
    line_num--;
    column = _buffer[line_num].size() - 1; /* null terminated */
  }
  else if (back) {
    column--;
  }

  // Check if we're at the end of a line, and should merge them
  if (column == _buffer[line_num].size() - 1 /* null terminated */) {
    auto& currline = _buffer[line_num];
    auto& nextline = _buffer[line_num + 1];
    currline.pop_back();
    currline.insert(currline.end(), nextline.begin(), nextline.end());
    _buffer.erase(_buffer.begin() + line_num+1);
  }
  // Just a normal delete in the middle of the row.
  else {
    auto& currline = _buffer[line_num];
    currline.erase(currline.begin() + column);
  }

}
