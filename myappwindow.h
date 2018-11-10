//
// Created by mats on 2018-11-09.
//

#pragma once

#include <gtkmm.h>

class MyAppWindow : public Gtk::ApplicationWindow {
public:
    MyAppWindow(BaseObjectType* cobject,
    const Glib::RefPtr<Gtk::Builder>& refBuilder);

    static MyAppWindow* create();

    void open_file_view(const Glib::RefPtr<Gio::File>& file);

protected:
    Glib::RefPtr<Gtk::Builder> m_refBuilder;
    Gtk::Box* m_box;
    Gtk::Scrollbar* m_scrollbar;
    Glib::RefPtr<Gtk::Adjustment> m_adjustment;
};

