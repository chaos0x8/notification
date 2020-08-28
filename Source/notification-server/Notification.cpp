#include "Notification.hpp"
#include <boost/process/spawn.hpp>
#include <cassert>
#include <stdexcept>

namespace Notify {
  void init(std::string appName) {
    notify_init(appName.c_str());
  }

  Notification::Action::Action(std::string label, std::string cmd)
    : label(label), cmd(cmd) {}

  void Notification::show() {
    assert(note == nullptr);

    note = notify_notification_new(name.c_str(), content.c_str(), icon.c_str());
    notify_notification_set_timeout(note, timeout.count());
    notify_notification_set_category(note, "Testing Notifications");
    notify_notification_set_urgency(note, NOTIFY_URGENCY_NORMAL);

    if (action) {
      notify_notification_add_action(note, "run", action->label.c_str(),
        &Notification::execAction, new Action(*action),
        &Notification::freeAction);
    }

    GError* error = nullptr;
    notify_notification_show(note, &error);

    action = std::nullopt;

    if (error != nullptr) {
      throw std::runtime_error("error during 'notify_notification_show");
    }
  }

  void Notification::execAction(NotifyNotification*, char*, gpointer data) {
    auto action = static_cast<Action*>(data);

    const auto pos = action->cmd.find_first_of(" ");
    if (pos != std::string::npos) {
      auto cmd = action->cmd.substr(0, pos);
      auto arg = action->cmd.substr(pos + 1, action->cmd.size() - pos - 1);
      boost::process::spawn(cmd, arg);
    } else {
      boost::process::spawn(action->cmd);
    }
  }

  void Notification::freeAction(gpointer data) {
    delete static_cast<Action*>(data);
  }

  const std::string Notification::ICON_INFO = "info";
} // namespace Notify
