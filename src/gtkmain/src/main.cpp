#include <gtkmm/application.h>
#include <gtkmm/window.h>

#include "myapplication.h"

int main(int argc, char* argv[])
{
  /*
  auto app = Gtk::Application::create(argc, argv, "org.gtkmm.example", Gio::APPLICATION_HANDLES_OPEN);

  const char* filename = "/home/mats/code/hello-world-gtk/CMakeLists.txt";
  MyWindow window(filename);
  window.set_title("Drawing text example");

  window.set_modal(true);
  return app->run(window);
   */
  auto app = MyApplication::create();
  return app->run(argc, argv);
}