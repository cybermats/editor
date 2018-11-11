//
// Created by mats on 11/10/18.
//

#pragma once


#include "style/style.h"

class Formatter {
public:
  virtual StyleList format_line(const char* str) = 0;
    virtual ~Formatter() = default;

protected:
    Formatter() = default;


};
