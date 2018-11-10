//
// Created by mats on 2018-11-06.
//

#pragma once
#include <gtkmm/drawingarea.h>
#include <gtkmm/scrollable.h>
#include <gtkmm/builder.h>
#include <gtkmm.h>
#include "mytextbuffer.h"

class MyTextArea : public Gtk::DrawingArea {
public:
    MyTextArea(BaseObjectType* cobject,
    const Glib::RefPtr<Gtk::Builder>& refBuilder);

    MyTextArea();

    ~MyTextArea() override;

    void set_file(const std::string& filename);
    void set_adjustment(Glib::RefPtr<Gtk::Adjustment> adjustment);
    void on_adjustment_value_changed();

protected:
    //Override default signal handler:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
    bool on_key_press_event(GdkEventKey* event) override;
    bool on_button_press_event(GdkEventButton *button_event) override;
    void on_realize() override;

    bool on_scroll_event(GdkEventScroll *scroll_event) override;

private:
    void draw_line(const Cairo::RefPtr<Cairo::Context>& cr, unsigned long line_num, double x, double y);

    void initialize();

    enum caret_pos_t {
        KNOWN,
        END
    };
    Glib::RefPtr<Gtk::Builder> m_refBuilder;
    Glib::RefPtr<Gtk::Adjustment> m_adjustment;

    bool move_caret_up();
    bool move_caret_down();
    bool move_caret_left();
    bool move_caret_right();

    const char* _font_name = "DejaVu Sans Mono";
    const int _font_size = 10;
    const Pango::Weight _font_weight = Pango::WEIGHT_NORMAL;

    MyTextBuffer _buffer;
    std::vector<std::vector<char>> _screen_lines;
    std::vector<unsigned long> _screen_line_sizes;
    unsigned long _current_line_count = 0;
    unsigned long _top_line = 0;
    unsigned long _top_line_new = 0;


    Pango::FontDescription _font_description;
    int _char_height = 0;
    int _char_width = 0;
    unsigned long _caret_line = 0;
    unsigned long _caret_column = 0;
    caret_pos_t _caret_pos = KNOWN;
};


