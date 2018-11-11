//
// Created by mats on 11/11/18.
//

#pragma once

#include <iostream>
#include <vector>
#include "stylelist.h"

class Formatter;
class ForegroundStyle;
class BackgroundStyle;
class BoldStyle;



class Stylizer {

public:
    explicit Stylizer(Formatter* formatter);
    virtual ~Stylizer();

    virtual void visit(const ForegroundStyle& style) = 0;

    virtual void visit(const BackgroundStyle& style) = 0;

    virtual void visit(const BoldStyle& style) = 0;

    void process(const StyleList& styles);

protected:
    virtual void on_begin() = 0;
    virtual StyleList process_line(const char*);

private:
    Formatter* m_formatter;
};


