/*!
 *  \author <https://github.com/chaos0x8>
 *  \copyright
 *  Copyright (c) 2015, <https://github.com/chaos0x8>
 *
 *  \copyright
 *  Permission to use, copy, modify, and/or distribute this software for any
 *  purpose with or without fee is hereby granted, provided that the above
 *  copyright notice and this permission notice appear in all copies.
 *
 *  \copyright
 *  THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 *  WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 *  MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 *  ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 *  WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 *  ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 *  OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "Notification.hpp"
#include <stdexcept>
#include <cassert>
#include <boost/process/spawn.hpp>

namespace Notify
{
  void init(std::string appName)
  {
    notify_init(appName.c_str());
  }

  Notification::Action::Action(std::string label, std::string cmd)
    : label(label), cmd(cmd)
  {
  }

  void Notification::show()
  {
    assert(note == nullptr);

    note = notify_notification_new(name.c_str(), content.c_str(), icon.c_str());
    notify_notification_set_timeout(note, timeout.count());
    notify_notification_set_category(note, "Testing Notifications");
    notify_notification_set_urgency(note, NOTIFY_URGENCY_NORMAL);

    if (action)
    {
      notify_notification_add_action(
          note,
          "run",
          action->label.c_str(),
          &Notification::execAction,
          new Action(*action),
          &Notification::freeAction);
    }

    GError* error = nullptr;
    notify_notification_show(note, &error);

    action = std::nullopt;

    if (error != nullptr)
    {
      throw std::runtime_error("error during 'notify_notification_show");
    }
  }

  void Notification::execAction(NotifyNotification*, char*, gpointer data)
  {
    auto action = static_cast<Action*>(data);

    const auto pos = action->cmd.find_first_of(" ");
    if (pos != std::string::npos)
    {
      auto cmd = action->cmd.substr(0, pos);
      auto arg = action->cmd.substr(pos+1, action->cmd.size()-pos-1);
      boost::process::spawn(cmd, arg);
    }
    else
    {
      boost::process::spawn(action->cmd);
    }
  }

  void Notification::freeAction(gpointer data)
  {
    delete static_cast<Action*>(data);
  }

  const std::string Notification::ICON_INFO = "info";
}
