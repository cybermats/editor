
#include "formatter/selectionformatter.h"
#include "mytextview.h"
#include <cstring>




void SelectionFormatter::format_line(
    StyleList &styleList, unsigned long line, const char *str) {
  auto begin = m_textView->get_selection_begin();
  auto end = m_textView->get_selection_end();
  if (line < begin.line || line > end.line)
    return;
  unsigned long start_col = 0;
  unsigned long end_col = strlen(str);
  if (line == begin.line)
    start_col = begin.column;
  if (line == end.line)
    end_col = end.column;

  styleList.add(Style::createBackground(128, 128, 255, start_col, end_col));
}
