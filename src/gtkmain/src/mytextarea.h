//
// Created by mats on 2018-11-06.
//

#pragma once
#include <gtkmm/drawingarea.h>
#include <gtkmm/scrollable.h>
#include <gtkmm/builder.h>
#include <gtkmm.h>
#include "mytextview.h"
#include "pangostylizer.h"
#include <formatter/regexformatter.h>

class MyTextArea : public Gtk::DrawingArea {
public:
    MyTextArea(BaseObjectType* cobject,
    const Glib::RefPtr<Gtk::Builder>& refBuilder);

    ~MyTextArea() override;

    void set_file(const std::string& filename);
    void set_adjustment(Glib::RefPtr<Gtk::Adjustment> adjustment);
    void on_adjustment_value_changed();

protected:
    //Override default signal handler:
    bool on_draw(const Cairo::RefPtr<Cairo::Context>& cr) override;
    bool on_key_press_event(GdkEventKey* event) override;
    bool on_button_press_event(GdkEventButton *button_event) override;
    bool on_button_release_event(GdkEventButton *button_event) override;
    void on_realize() override;

  bool on_scroll_event(GdkEventScroll *scroll_event) override;
  bool on_motion_notify_event(GdkEventMotion *motion_event) override;

    static void on_commit_callback(GtkIMContext* context, gchar* str, gpointer user_data);
    void on_commit(gchar        *str);

private:
    void draw_line(const Cairo::RefPtr<Cairo::Context>& cr, unsigned long line_num, double x, double y);

    void initialize();

    Glib::RefPtr<Gtk::Builder> m_refBuilder;
    Glib::RefPtr<Gtk::Adjustment> m_adjustment;

    MyTextView m_textView;
    RegexFormatter m_formatter;
    PangoStylizer m_stylizer;


    int _char_height = 0;
    int _char_width = 0;

    GtkIMContext* m_imContext;
};
