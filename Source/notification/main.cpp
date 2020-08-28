#include "Application.hpp"
#include "Notification.hpp"
#include "c8-common/errors/SystemError.hpp"
#include "c8-option-parser.hpp"
#include <gtk/gtk.h>
#include <iostream>

enum class Tag : uint32_t { Action, ActionLabel };

int main(int argc, char** argv) {
  C8::OptionParser::Parser op;
  auto action = op.on<std::string>(
    "--action", "Command to execute (works only when server is active");
  auto actionLabel = op.on<std::string>("--action-label",
    "Label for action to execute (works only when server is active)");
  auto help = op.on<bool>("--help", "Displays help");

  try {
    using namespace std::string_literals;

    op.parse(argc, argv);
    op.banner("Usage: "s + std::string(op.name()) +
              "notification [options] title content"s);

    if (help == true) {
      std::cout << op.help() << std::endl;
      return -1;
    }

    try {
      return Application::notifyViaServer(*action, *actionLabel, op.args());
    } catch (const C8::Common::Errors::SystemError&) {
      gtk_init(&argc, &argv);
      Notify::init("notification");
      return Application::notify(op.args());
    }
  } catch (const C8::OptionParser::Error& e) {
    std::cerr << e.what() << std::endl << std::endl << op.help();
    return -1;
  } catch (std::exception& e) {
    std::cerr << "Exception: '" << e.what() << "'" << std::endl;
    return -1;
  }
}
