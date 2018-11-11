//
// Created by mats on 11/11/18.
//

#pragma once

#include "stylizer.h"

class Style {
public:
    virtual ~Style() = default;

protected:
    Style(unsigned long start_idx, unsigned long end_idx)
            : m_start_idx(start_idx), m_end_idx(end_idx) {}


public:
    unsigned long get_start_idx() const {
      return m_start_idx;
    }

    void set_start_idx(unsigned long m_start_idx) {
      Style::m_start_idx = m_start_idx;
    }

    unsigned long get_end_idx() const {
      return m_end_idx;
    }

    void set_end_idx(unsigned long m_end_idx) {
      Style::m_end_idx = m_end_idx;
    }


private:
    friend void Stylizer::process(const StyleList& styles);
    virtual void accept(Stylizer* stylizer) const = 0;

    unsigned long m_start_idx;
    unsigned long m_end_idx;
};


class ForegroundStyle: public Style {
protected:
    ForegroundStyle(unsigned char red, unsigned char green, unsigned char blue, unsigned long start, unsigned long end)
            : Style(start, end)
            , m_red(red), m_green(green), m_blue(blue)
    {}

public:
    static ForegroundStyle* create(unsigned char red,
                                   unsigned char green,
                                   unsigned char blue,
                                   unsigned long start = 0,
                                   unsigned long end = 0)
    {
      return new ForegroundStyle(red, green, blue, start, end);
    }

    unsigned char get_red() const {
      return m_red;
    }

    void set_red(unsigned char red) {
      m_red = red;
    }

    unsigned char get_green() const {
      return m_green;
    }

    void set_green(unsigned char green) {
      m_green = green;
    }

    unsigned char get_blue() const {
      return m_blue;
    }

    void set_blue(unsigned char blue) {
      m_blue = blue;
    }

    void accept(Stylizer* stylizer) const override;

private:
    unsigned char m_red;
    unsigned char m_green;
    unsigned char m_blue;
};

class BackgroundStyle: public Style {
protected:
    BackgroundStyle(unsigned char red, unsigned char green, unsigned char blue, unsigned long start, unsigned long end)
            : Style(start, end)
            , m_red(red), m_green(green), m_blue(blue)
    {}

public:
    static BackgroundStyle* create(unsigned char red,
                                   unsigned char green,
                                   unsigned char blue,
                                   unsigned long start = 0,
                                   unsigned long end = 0)
    {
      return new BackgroundStyle(red, green, blue, start, end);
    }

    unsigned char get_red() const {
      return m_red;
    }

    void set_red(unsigned char red) {
      m_red = red;
    }

    unsigned char get_green() const {
      return m_green;
    }

    void set_green(unsigned char green) {
      m_green = green;
    }

    unsigned char get_blue() const {
      return m_blue;
    }

    void set_blue(unsigned char blue) {
      m_blue = blue;
    }

    void accept(Stylizer* stylizer) const override;


private:
    unsigned char m_red;
    unsigned char m_green;
    unsigned char m_blue;
};

class BoldStyle: public Style {
public:
    static BoldStyle* create(unsigned long start = 0,
                                   unsigned long end = 0)
    {
      return new BoldStyle(start, end);
    }

    BoldStyle(unsigned long start, unsigned long end)
    : Style(start, end) {}

    void accept(Stylizer* stylizer) const override;
};