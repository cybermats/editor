//
// Created by mats on 11/11/18.
//

#pragma once

#include "style.h"
#include "stylelist.h"
#include <functional>

class StyleFactory {
public:
    static StyleFactory build() {
      return StyleFactory();
    }

    StyleFactory& addForeground(unsigned char red, unsigned char  green, unsigned char  blue)
    {
      m_styles.emplace_back([red, green, blue](auto start, auto end){
          return ForegroundStyle::create(red, green, blue, start, end);
      });

      return *this;
    }

    StyleFactory& addBackground(unsigned char  red, unsigned char  green, unsigned char  blue)
    {
      m_styles.emplace_back([red, green, blue](auto start, auto end){
          return BackgroundStyle::create(red, green, blue, start, end);
      });
      return *this;
    }

    StyleFactory& addBold()
    {
      m_styles.emplace_back([](auto start, auto end){
          return BoldStyle::create(start, end);
      });
      return *this;
    }

    StyleList get(unsigned long start, unsigned long end)
    {
      StyleList output;
      for(const auto &f: m_styles) {
        output.add(f(start, end));
      }
      return output;
    }
private:
    std::vector<std::function<Style*(unsigned long, unsigned long)>> m_styles;

};


