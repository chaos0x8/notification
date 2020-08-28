#pragma once

#include <chrono>
#include <libnotify/notify.h>
#include <optional>
#include <string>

namespace Notify {
  void init(std::string appName);

  class Notification {
  public:
    struct Action {
      Action(std::string label, std::string cmd);

      std::string label;
      std::string cmd;
    };

    void show();

    static const std::string ICON_INFO;

    std::string name;
    std::string content;
    std::string icon;
    std::chrono::milliseconds timeout{1000};

    std::optional<Action> action;

  private:
    static void execAction(NotifyNotification*, char*, gpointer);
    static void freeAction(gpointer);

    NotifyNotification* note{nullptr};
  };
} // namespace Notify
