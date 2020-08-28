#pragma once

#include <chrono>
#include <libnotify/notify.h>
#include <string>

namespace Notify {
  void init(std::string appName);

  class Notification {
  public:
    void show();

    std::string name;
    std::string content;
    std::string icon;
    std::chrono::milliseconds timeout{1000};

    static const std::string ICON_INFO;

  private:
    NotifyNotification* note{nullptr};
  };
} // namespace Notify
