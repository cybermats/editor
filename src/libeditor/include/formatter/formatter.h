//
// Created by mats on 11/10/18.
//

#pragma once


class StyleList;

class Formatter {
public:
  virtual void format_line(
      StyleList& styleList, unsigned long line, const char* str) = 0;
    virtual ~Formatter() = default;

protected:
    Formatter() = default;


};
