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

#include "Application.hpp"
#include "Notification.hpp"
#include "Messages.hpp"
#include "Raw.hpp"
#include "Config.hpp"
#include <OptionParser/Argv.hpp>
#include <Network.hpp>
#include <stdexcept>

int Application::notifyViaServer(Common::OptionParser::Argv& ARGV)
{
  auto client = Common::Network::TcpIpClient("localhost", Config::PORT);

  std::string title = ARGV.take();
  std::string content = ARGV.take();

  auto message = Message();
  message.type = Message::Type::Notification;
  message.notification.titleSize = title.size();
  message.notification.contentSize = content.size();

  auto raw = Raw();
  raw.insert(&message);
  raw.insert(title.data(), title.size());
  raw.insert(content.data(), content.size());

  client.send(raw.data(), raw.size());
  client.shutdown();

  return 0;
}

int Application::notify(Common::OptionParser::Argv& ARGV)
{
  using namespace std::chrono;

  Notify::Notification note;
  note.name = ARGV.take();
  note.content = ARGV.take();
  note.icon = Notify::Notification::ICON_INFO;
  note.timeout = duration_cast<milliseconds>(seconds(5));
  note.show();

  return 0;
}

