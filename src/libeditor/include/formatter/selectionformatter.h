//
// Created by mats on 11/11/18.
//

#pragma once


#include "formatter.h"
#include "style/stylelist.h"

class MyTextView;

class SelectionFormatter : public Formatter {
public:
  SelectionFormatter(const MyTextView *textView)
      : m_textView(textView)
  {}

  void format_line(StyleList &styleList, unsigned long line, const char *str) override;

private:
  const MyTextView *m_textView = nullptr;
};
