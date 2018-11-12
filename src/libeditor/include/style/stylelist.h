//
// Created by mats on 11/11/18.
//

#pragma once
#include <vector>
#include <memory>

#include "style/style.h"


class StyleList {
public:

    void add(const Style& style) {
      m_styles.push_back(style);
    }

    const Style& operator[](unsigned long idx) const {
      return m_styles[idx];
    }

    unsigned long size() const {
      return m_styles.size();
    }

private:
    std::vector<Style> m_styles;
};
