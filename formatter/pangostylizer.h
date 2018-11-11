//
// Created by mats on 11/11/18.
//

#pragma once


#include "stylizer.h"
#include <pangomm.h>

class PangoStylizer : public Stylizer {
public:
    explicit PangoStylizer(Formatter* formatter);
    ~PangoStylizer() override;

    void visit(const ForegroundStyle& style) override;

    void visit(const BackgroundStyle& style) override;

    void visit(const BoldStyle& style) override;

    const Pango::FontDescription& get_font_description() const;
    void format(Glib::RefPtr<Pango::Layout> layout, const char *line);

    protected:
    void on_begin() override;

private:
    Pango::AttrList m_attrs;
    const char* _font_name = "DejaVu Sans Mono";
    const int _font_size = 10;
    const Pango::Weight _font_weight = Pango::WEIGHT_NORMAL;

    Pango::FontDescription _font_description;


};


