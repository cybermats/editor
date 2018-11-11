//
// Created by mats on 11/11/18.
//

#pragma once

#include <map>
#include <utility>
#include "stylefactory.h"

class StyleMap {
public:
    void add(const std::string &name, StyleFactory styleFactory) {
      m_map[name] = std::move(styleFactory);
    }

    void populate(StyleList& styleList, const std::string &name, unsigned long start, unsigned long end) {
      m_map[name].populate(styleList, start, end);

    }

private:
    std::map<std::string, StyleFactory> m_map;
};


