//
// Created by mats on 2018-11-09.
//

#pragma once

#include <gtkmm.h>

class MyAppWindow;

class MyApplication : public Gtk::Application {
protected:
    MyApplication();

public:
    static Glib::RefPtr<MyApplication> create();

protected:
    void on_open(const type_vec_files &files, const Glib::ustring &hint) override;

    void on_activate() override;

private:
    MyAppWindow* create_appwindow();
    void on_hide_window(Gtk::Window* window);
};


