//
// Created by mats on 11/11/18.
//

#pragma once
#include <vector>

class Style;

class StyleList {
public:
    StyleList() = default;
    ~StyleList() {
      //TODO(mats): Figure out how the life cycle of Styles should be.
      /*      for(auto s: m_styles)
        delete s;*/
    }

    void add(Style* style);

    void add(const StyleList& styleList);

    const Style* operator[](unsigned long idx) const {
      return m_styles[idx];
    }

    unsigned long size() const {
      return m_styles.size();
    }

private:
    std::vector<Style*> m_styles;
};
