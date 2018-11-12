//
// Created by mats on 11/11/18.
//

#pragma once


#include "formatter.h"
#include "style/stylemap.h"
#include "style/stylelist.h"
#include <regex>


class RegexFormatter : public Formatter {
public:
  void format_line(StyleList &styleList, unsigned long line, const char *str) override;

    RegexFormatter();

private:
    std::regex from_keywords(const std::string &keywords) {
      std::string results = std::regex_replace(keywords, std::regex("^\\s+|\\s+$", std::regex_constants::ECMAScript), "");
      results = std::regex_replace(results, std::regex("\\s+", std::regex_constants::ECMAScript), "|");
      std::string output ="\\b(?:" + results + ")\\b";
      return std::regex(output, std::regex_constants::ECMAScript);
    }

  std::vector<std::pair<std::string, std::regex>> m_regexs;
  StyleMap m_styleMap;
};
