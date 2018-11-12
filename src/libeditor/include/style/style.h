//
// Created by mats on 11/11/18.
//

#pragma once

class StyleType;
class Stylizer;

class Style
{
 public:
  Style(const Style& style);
  Style& operator=(Style style);

  ~Style();
  static Style createForeground(
      unsigned long red,
      unsigned long green,
      unsigned long blue,
      unsigned long start,
      unsigned long end);

  static Style createBackground(
      unsigned long red,
      unsigned long green,
      unsigned long blue,
      unsigned long start,
      unsigned long end);

  static Style createBold(
      unsigned long start,
      unsigned long end);

  void accept(Stylizer* stylizer) const;

 private:
  Style(StyleType *style_type);
  StyleType *m_style_type;
};
