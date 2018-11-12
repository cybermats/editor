//
// Created by mats on 11/11/18.
//

#pragma once

#include <iostream>
#include <vector>
#include <memory>
#include "stylelist.h"

class Formatter;
class ForegroundStyleType;
class BackgroundStyleType;
class BoldStyleType;



class Stylizer {

public:
  virtual ~Stylizer() = default;

  void add_formatter(Formatter *formatter);

    virtual void visit(const ForegroundStyleType* style) = 0;

    virtual void visit(const BackgroundStyleType* style) = 0;

    virtual void visit(const BoldStyleType* style) = 0;

    void process(const StyleList& styles);

protected:
    virtual void on_begin() = 0;
  virtual StyleList process_line(unsigned long line_num, const char* str);

private:
  std::vector<std::unique_ptr<Formatter>> m_formatters;
};
