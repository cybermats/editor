//
// Created by mats on 2018-11-09.
//

#include <iostream>
#include "myapplication.h"
#include "myappwindow.h"

void MyApplication::on_open(const Gio::Application::type_vec_files &files, const Glib::ustring &/*hint*/) {
  MyAppWindow* appWindow = nullptr;
  auto windows = get_windows();

  if (!windows.empty())
    appWindow = dynamic_cast<MyAppWindow*>(windows[0]);

  try {
    if (!appWindow)
      appWindow = create_appwindow();

    for (auto &file : files) {
      appWindow->open_file_view(file);
      break;
    }

    appWindow->present();
  }
  catch (const Glib::Error& ex)
  {
    std::cerr << "MyApplication::on_open(): " << ex.what() << std::endl;
  }
  catch (const std::exception& ex)
  {
    std::cerr << "MyApplication::on_open(): " << ex.what() << std::endl;
  }
}

void MyApplication::on_activate() {
  try {
    auto appWindow = create_appwindow();
    appWindow->present();
  }
  // If create_appwindow() throws an exception (perhaps from Gtk::Builder),
  // no window has been created, no window has been added to the application,
  // and therefore the application will stop running.
  catch (const Glib::Error& ex)
  {
    std::cerr << "MyApplication::on_activate(): " << ex.what() << std::endl;
  }
  catch (const std::exception& ex)
  {
    std::cerr << "MyApplication::on_activate(): " << ex.what() << std::endl;
  }
}

MyApplication::MyApplication()
        : Gtk::Application("org.mats.example.hello-world", Gio::APPLICATION_HANDLES_OPEN) {

}

Glib::RefPtr<MyApplication> MyApplication::create() {
  return Glib::RefPtr<MyApplication>(new MyApplication());
}

MyAppWindow *MyApplication::create_appwindow() {
  auto appWindow = MyAppWindow::create();

  add_window(*appWindow);

  appWindow->signal_hide().connect(sigc::bind<Gtk::Window*>(
          sigc::mem_fun(*this, &MyApplication::on_hide_window),
          appWindow));

  return appWindow;
}

void MyApplication::on_hide_window(Gtk::Window *window) {
  delete window;
}
