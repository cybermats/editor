//
// Created by mats on 11/11/18.
//

#pragma once


#include <style/stylizer.h>
#include <pangomm.h>
#include <formatter/formatter.h>

class PangoStylizer : public Stylizer {
public:
    explicit PangoStylizer();
    ~PangoStylizer() override;

    void visit(const ForegroundStyleType* style) override;

    void visit(const BackgroundStyleType* style) override;

    void visit(const BoldStyleType* style) override;

    const Pango::FontDescription& get_font_description() const;
    void format(Glib::RefPtr<Pango::Layout> layout,
                unsigned long line_num, const char *line);

    protected:
    void on_begin() override;

private:
    Pango::AttrList m_attrs;
    const char* _font_name = "DejaVu Sans Mono";
    const int _font_size = 10;
    const Pango::Weight _font_weight = Pango::WEIGHT_NORMAL;

    Pango::FontDescription _font_description;
};
