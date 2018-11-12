//
// Created by mats on 11/11/18.
//

#include "style/style.h"
#include "style/styletype.h"

Style::Style(const Style& style) {
  m_style_type = style.m_style_type->clone();
}

Style& Style::operator=(Style style) {
  m_style_type = style.m_style_type;
  return *this;
}

Style::~Style() {
  delete m_style_type;
  m_style_type = nullptr;
}

Style Style::createForeground(
    unsigned long red,
    unsigned long green,
    unsigned long blue,
    unsigned long start,
    unsigned long end) {
  return Style(new ForegroundStyleType(red, green, blue, start, end));
}

Style Style::createBackground(
    unsigned long red,
    unsigned long green,
    unsigned long blue,
    unsigned long start,
    unsigned long end) {
  return Style(new BackgroundStyleType(red, green, blue, start, end));
}

Style Style::createBold(
    unsigned long start,
    unsigned long end) {
  return Style(new BoldStyleType(start, end));
}

void Style::accept(Stylizer* stylizer) const {
  m_style_type->accept(stylizer);
}

Style::Style(StyleType *style_type) {
  m_style_type = style_type;
}
