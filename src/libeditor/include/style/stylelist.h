//
// Created by mats on 11/11/18.
//

#pragma once
#include <vector>
#include <memory>

class Style;

class StyleList {
public:

    void add(Style* style) {
      m_styles.push_back(std::unique_ptr<Style>(style));
    }

    const Style* operator[](unsigned long idx) const {
      return m_styles[idx].get();
    }

    unsigned long size() const {
      return m_styles.size();
    }

private:
    std::vector<std::unique_ptr<Style>> m_styles;
};
