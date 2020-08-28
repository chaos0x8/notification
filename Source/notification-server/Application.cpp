#include "Application.hpp"
#include "Config.hpp"
#include "Messages.hpp"
#include "Notification.hpp"
#include <chrono>
#include <csignal>
#include <gtk/gtk.h>
#include <iostream>
#include <memory>

extern "C" void stopOther() {
  try {
    auto client = C8::Network::TcpIpClient("localhost", Config::PORT);

    auto message = Message();
    message.type = Message::Type::Command;
    message.command.type = Command::Type::Stop;

    client.send(&message, sizeof(message));
    client.shutdown();
  } catch (const C8::Common::Errors::SystemError&) {
  }
}

extern "C" void signalHandler(int) {
  ::stopOther();
}

int Application::run(C8::OptionParser::Args args) {
  try {
    auto app = std::make_unique<Application>(std::move(args));
    return app->run();
  } catch (const C8::Common::Errors::SystemError& e) {
    const auto ADDRESS_ALREADY_IN_USE = 98;
    if (e.errnoValue() == ADDRESS_ALREADY_IN_USE)
      return 1;
    throw;
  }
}

void Application::stop() {
  ::stopOther();
}

Application::Application(C8::OptionParser::Args args)
  : args(std::move(args)), server("localhost", Config::PORT) {
  selector.add(server, &Application::serverProc, this);
}

Application::~Application() = default;

int Application::run() {
  std::signal(SIGINT, signalHandler);
  std::signal(SIGTERM, signalHandler);

  selector.on_finish([] { gtk_main_quit(); });

  gtk_main();

  return 0;
}

void Application::serverProc() {
  clients.push_back(server.accept());
  selector.add(clients.back(), &Application::clientProc, this,
    clients.back().getNativeHandler());
}

void Application::clientProc(C8::Network::NativeHandler nativeHandler) {
  using namespace std::chrono;

  auto client = std::find_if(
    std::begin(clients), std::end(clients), [nativeHandler](const auto& cl) {
      return cl.getNativeHandler() == nativeHandler;
    });

  if (client == std::end(clients)) {
    return;
  }

  auto message = Message();

  if (client->receive(&message, sizeof(message))) {
    switch (message.type) {
    case Message::Type::Command:
      switch (message.command.type) {
      case Command::Type::Stop:
        std::cout << "stopping\n";
        selector.stop();
        break;
      default:
        break;
      }
      break;
    case Message::Type::Notification: {
      auto title = std::string(message.notification.titleSize, '\0');
      client->receive(title.data(), title.size());

      auto content = std::string(message.notification.contentSize, '\0');
      client->receive(content.data(), content.size());

      auto actionLabel =
        std::string(message.notification.actionLabelSize, '\0');
      client->receive(actionLabel.data(), actionLabel.size());

      auto action = std::string(message.notification.actionSize, '\0');
      client->receive(action.data(), action.size());

      Notify::Notification note;
      note.name = title;
      note.content = content;
      note.icon = Notify::Notification::ICON_INFO;
      note.timeout = duration_cast<milliseconds>(seconds(5));

      if (actionLabel.size() > 0 and action.size() > 0) {
        note.action = Notify::Notification::Action(actionLabel, action);
      }

      note.show();
      break;
    }
    default:
      break;
    }
  } else {
    client->shutdown();
    selector.remove(*client);
    clients.erase(client);
  }
}
