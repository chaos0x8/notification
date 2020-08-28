#pragma once

#include "c8-network.hpp"
#include "c8-option-parser.hpp"

struct Application {
  static int run(C8::OptionParser::Args);
  static void stop();

  Application(C8::OptionParser::Args);
  ~Application();

  int run();

private:
  void serverProc();
  void clientProc(C8::Network::NativeHandler nativeHandler);

  C8::OptionParser::Args args;

  std::vector<C8::Network::TcpIpClient> clients;
  C8::Network::TcpIpServer server;

  C8::Network::Selector selector;
};
