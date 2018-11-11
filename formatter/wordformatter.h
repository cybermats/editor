//
// Created by mats on 11/10/18.
//

#pragma once

#include <vector>
#include <cstring>
#include "formatter.h"
#include "style.h"

class WordFormatter : public Formatter {
public:
    StyleList format_line(const char* str) override {
    StyleList output;
    int start = -1;
    int len = strlen(str);

    for (int i = 0; i < len; ++i) {
      char c = str[i];
      if (c == ' ') {
        if (start >= 0) {
          output.add(BackgroundStyle::create(255, 0, 0, start, i));
          start = -1;
        }
      } else {
        if (start < 0) {
          start = i;
        }
      }
    }
    if (start >= 0) {
      output.add(BackgroundStyle::create(255, 0, 0, start, len));
    }
    return output;
  }
};


