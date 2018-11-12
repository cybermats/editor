//
// Created by mats on 11/11/18.
//

#include "style/style.h"
#include "style/stylizer.h"
#include "formatter/formatter.h"

void Stylizer::add_formatter(Formatter *formatter) {
  m_formatters.emplace_back(formatter);
}


void Stylizer::process(const StyleList& styles) {
  on_begin();
  for (unsigned long i = 0; i < styles.size(); ++i) {
    auto& style = styles[i];
    style.accept(this);
  }
}

StyleList Stylizer::process_line(unsigned long line_num, const char * str) {
  StyleList styleList;
  for (const auto &formatter : m_formatters) {
    formatter->format_line(styleList, line_num, str);
  }
  return styleList;
}
