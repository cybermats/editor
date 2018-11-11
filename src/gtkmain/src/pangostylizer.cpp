//
// Created by mats on 11/11/18.
//

#include "pangostylizer.h"
#include <style/style.h>
#include <gtkmm.h>

void PangoStylizer::visit(const ForegroundStyle& style) {
  auto attr = Pango::Attribute::create_attr_foreground(
          style.get_red()*256,
          style.get_green()*256,
          style.get_blue()*256);
  attr.set_start_index(style.get_start_idx());
  attr.set_end_index(style.get_end_idx());
  m_attrs.insert(attr);

}

void PangoStylizer::visit(const BackgroundStyle& style) {
  auto attr = Pango::Attribute::create_attr_background(
          style.get_red()*256,
          style.get_green()*256,
          style.get_blue()*256);
  attr.set_start_index(style.get_start_idx());
  attr.set_end_index(style.get_end_idx());
  m_attrs.insert(attr);
}

void PangoStylizer::visit(const BoldStyle &style) {
  auto attr = Pango::Attribute::create_attr_weight(Pango::Weight::WEIGHT_BOLD);
  attr.set_start_index(style.get_start_idx());
  attr.set_end_index(style.get_end_idx());
  m_attrs.insert(attr);
}

void PangoStylizer::on_begin() {
  m_attrs = Pango::AttrList();
}

PangoStylizer::~PangoStylizer() = default;

void PangoStylizer::format(Glib::RefPtr<Pango::Layout> layout, const char *line) {
  layout->set_font_description(_font_description);

  auto styles = process_line(line);
  process(styles);
  layout->set_attributes(m_attrs);
}

PangoStylizer::PangoStylizer(Formatter *formatter) : Stylizer(formatter) {
  _font_description.set_family(_font_name);
  _font_description.set_weight(_font_weight);
  _font_description.set_size(_font_size * PANGO_SCALE);
}

const Pango::FontDescription &PangoStylizer::get_font_description() const {
  return _font_description;
}
