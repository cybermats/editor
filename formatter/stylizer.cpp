//
// Created by mats on 11/11/18.
//

#include "style.h"
#include "stylizer.h"
#include "formatter.h"

void Stylizer::process(const StyleList& styles) {
  on_begin();
  for (unsigned long i = 0; i < styles.size(); ++i) {
    auto style = styles[i];
    style->accept(this);
  }
}

Stylizer::Stylizer(Formatter * formatter)
: m_formatter(formatter)
{}

Stylizer::~Stylizer() {
  delete m_formatter;
}

StyleList Stylizer::process_line(const char * line) {
  return m_formatter->format_line(line);
}
