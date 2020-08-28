#pragma once

#include "c8-option-parser.hpp"
#include <string_view>

class Application {
public:
  static int notifyViaServer(std::string_view action,
    std::string_view actionLabel, C8::OptionParser::Args);
  static int notify(C8::OptionParser::Args);
};
