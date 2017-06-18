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

#pragma once

#include <libnotify/notify.h>
#include <string>
#include <chrono>
#include <optional>

namespace Notify
{
  void init(std::string appName);

  class Notification
  {
  public:
    struct Action
    {
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
}