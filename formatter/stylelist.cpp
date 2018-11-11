//
// Created by mats on 11/11/18.
//

#include "stylelist.h"
#include "style.h"


void StyleList::add(Style* style) {
  m_styles.push_back(style);
}

void StyleList::add(const StyleList& styleList) {
  m_styles.insert(m_styles.end(), styleList.m_styles.begin(), styleList.m_styles.end());
}
