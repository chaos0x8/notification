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
#include "Config.hpp"
#include "Messages.hpp"
#include "Notification.hpp"
#include <Common/Algorithm.hpp>
#include <OptionParser/Argv.hpp>
#include <csignal>
#include <memory>
#include <chrono>
#include <iostream>

extern "C" void stopOther()
{
  try
  {
    auto client = Common::Network::TcpIpClient("localhost", Config::PORT);

    auto message = Message();
    message.type = Message::Type::Command;
    message.command.type = Command::Type::Stop;

    client.send(&message, sizeof(message));
    client.shutdown();
  }
  catch (const Common::Exceptions::SystemError&)
  {
  }
}

extern "C" void signalHandler(int)
{
  ::stopOther();
}

int Application::run(Common::OptionParser::Argv& ARGV)
{
  try
  {
    auto app = std::make_unique<Application>(ARGV);
    return app->run();
  }
  catch (const Common::Exceptions::SystemError& e)
  {
    const auto ADDRESS_ALREADY_IN_USE = 98;
    if (e.errnoValue() == ADDRESS_ALREADY_IN_USE)
      return 1;
    throw;
  }
}

void Application::stop()
{
  ::stopOther();
}

Application::Application(Common::OptionParser::Argv& ARGV)
  : ARGV(ARGV),
    server("localhost", Config::PORT)
{
  selector.add(server, &Application::serverProc, this);
}

Application::~Application() = default;

int Application::run()
{
  std::signal(SIGINT, signalHandler);
  std::signal(SIGTERM, signalHandler);

  selector.wait();
  return 0;
}

void Application::serverProc()
{
  clients.push_back( server.accept() );
  selector.add( clients.back(), &Application::clientProc, this, clients.back().getNativeHandler() );
}

void Application::clientProc(Common::Network::NativeHandler nativeHandler)
{
  using namespace std::chrono;

  auto client = Common::Algorithm::first(clients, [nativeHandler](const auto& client) {
    return client.getNativeHandler() == nativeHandler;
  });

  auto message = Message();

  if (client->receive(&message, sizeof(message)))
  {
    switch (message.type)
    {
      case Message::Type::Command:
        switch (message.command.type)
        {
          case Command::Type::Stop:
            std::cout << "stopping\n";
            selector.stop();
            break;
          default:
            break;
        }
        break;
      case Message::Type::Notification:
      {
        auto title = std::string(message.notification.titleSize, '\0');
        client->receive(title.data(), title.size());

        auto content = std::string(message.notification.contentSize, '\0');
        client->receive(content.data(), content.size());

        Notify::Notification note;
        note.name = title;
        note.content = content;
        note.icon = Notify::Notification::ICON_INFO;
        note.timeout = duration_cast<milliseconds>(seconds(5));
        note.show();
        break;
      }
      default:
        break;
    }
  }
  else
  {
    client->shutdown();
    selector.remove(*client);
    clients.erase(client);
  }
}
