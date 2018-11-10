//
// Created by mats on 2018-11-09.
//

#include <iostream>
#include "myappwindow.h"
#include "mytextarea.h"

MyAppWindow::MyAppWindow(BaseObjectType* cobject,
                         const Glib::RefPtr<Gtk::Builder>& refBuilder)
                         : Gtk::ApplicationWindow(cobject)
                         , m_refBuilder(refBuilder)
        , m_box(nullptr)
        , m_scrollbar(nullptr)
        , m_adjustment( Gtk::Adjustment::create(0, 0, 0))
                         {
  m_refBuilder->get_widget("box4", m_box);
  if (!m_box)
    throw std::runtime_error("No \"box4\" object in window.ui");

  m_refBuilder->get_widget("scrollbar1", m_scrollbar);
  if (!m_scrollbar)
    throw std::runtime_error("No \"scrollbar1\" object in window.ui");
  m_scrollbar->set_adjustment(m_adjustment);

}

void MyAppWindow::open_file_view(const Glib::RefPtr<Gio::File> &file) {
  const auto basename = file->get_basename();
  const auto filepath = file->get_path();

  MyTextArea* area = nullptr;
  m_refBuilder->get_widget_derived("drawingarea1", area);
  if (!area)
    throw std::runtime_error("No \"drawingarea1\" object in window.ui");
  area->set_adjustment(m_adjustment);
  area->set_file(filepath);
}

MyAppWindow *MyAppWindow::create() {
  auto refBuilder = Gtk::Builder::create_from_resource("/org/mats/exampleapp/mywindow.ui");

  MyAppWindow* appWindow = nullptr;
  refBuilder->get_widget_derived("applicationwindow1", appWindow);

  if (!appWindow)
    throw std::runtime_error("No \"app_window\" object in window.ui");

  return appWindow;

}
