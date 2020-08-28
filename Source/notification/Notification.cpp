#include "Notification.hpp"
#include <cassert>
#include <stdexcept>

namespace Notify {
  const std::string Notification::ICON_INFO = "info";

  void init(std::string appName) {
    notify_init(appName.c_str());
  }

  void Notification::show() {
    assert(note == nullptr);

    note = notify_notification_new(name.c_str(), content.c_str(), icon.c_str());
    notify_notification_set_timeout(note, timeout.count());
    notify_notification_set_category(note, "Testing Notifications");
    notify_notification_set_urgency(note, NOTIFY_URGENCY_NORMAL);

    GError* error = nullptr;
    notify_notification_show(note, &error);

    if (error != nullptr) {
      throw std::runtime_error("error during 'notify_notification_show");
    }
  }
} // namespace Notify
