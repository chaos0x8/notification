#include "Application.hpp"
#include "Notification.hpp"
#include "c8-option-parser.hpp"
#include <gtk/gtk.h>
#include <iostream>

enum class Tag : uint32_t { Stop, Help };

int main(int argc, char** argv) {
  try {
    gtk_init(&argc, &argv);
    Notify::init("notification-server");

    C8::OptionParser::Parser op;
    auto stop = op.on<bool>("--stop", "stops running server");
    auto help = op.on<bool>("--help", "displays help");

    op.parse(argc, argv);

    if (help == true) {
      std::cout << op.help();
      return 0;
    } else if (stop) {
      Application::stop();
      return 0;
    } else {
      return Application::run(op.args());
    }
  } catch (std::exception& e) {
    std::cerr << "Exception: '" << e.what() << "'" << std::endl;
    return -1;
  }
}
