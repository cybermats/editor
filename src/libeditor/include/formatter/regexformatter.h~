//
// Created by mats on 11/11/18.
//

#pragma once


#include "formatter.h"
#include "stylemap.h"
#include "stylelist.h"
#include <regex>

class RegexFormatter : public Formatter {
public:
    StyleList format_line(const char *str) override;

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


