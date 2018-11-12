#include <gtkmm/application.h>
#include <gtkmm/window.h>

#include "myapplication.h"

int main(int argc, char* argv[])
{
  auto app = MyApplication::create();
  return app->run(argc, argv);
}
