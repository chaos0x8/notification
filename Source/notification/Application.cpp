#include "Application.hpp"
#include "Config.hpp"
#include "Messages.hpp"
#include "Notification.hpp"
#include "Raw.hpp"
#include "c8-network.hpp"
#include <stdexcept>

int Application::notifyViaServer(std::string_view action,
  std::string_view actionLabel, C8::OptionParser::Args args) {
  auto client = C8::Network::TcpIpClient("localhost", Config::PORT);

  std::string title = args.take();
  std::string content = args.take();

  auto message = Message();
  message.type = Message::Type::Notification;
  message.notification.titleSize = title.size();
  message.notification.contentSize = content.size();
  message.notification.actionLabelSize = actionLabel.size();
  message.notification.actionSize = action.size();

  auto raw = Raw();
  raw.insert(&message);
  raw.insert(title.data(), title.size());
  raw.insert(content.data(), content.size());
  raw.insert(actionLabel.data(), actionLabel.size());
  raw.insert(action.data(), action.size());

  client.send(raw.data(), raw.size());
  client.shutdown();

  return 0;
}

int Application::notify(C8::OptionParser::Args args) {
  using namespace std::chrono;

  Notify::Notification note;
  note.name = args.take();
  note.content = args.take();
  note.icon = Notify::Notification::ICON_INFO;
  note.timeout = duration_cast<milliseconds>(seconds(5));
  note.show();

  return 0;
}
