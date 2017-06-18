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
#include <OptionParser.hpp>
#include <Common/Exceptions.hpp>
#include <iostream>
#include <gtk/gtk.h>

enum class Tag : uint32_t
{
};

int main(int argc, char** argv)
{
  using namespace Common::OptionParser;

  auto parser = makeParser<Tag>();
  parser.addHelpPrefix("Usage: notification [options] title content");
  parser.parse(&argc, argv);

  auto ARGV = Argv(&argc, argv);

  try
  {
    try
    {
      return Application::notifyViaServer(ARGV);
    }
    catch (const Common::Exceptions::SystemError&)
    {
      gtk_init(&argc, &argv);
      return Application::notify(ARGV);
    }
  }
  catch (const OPError& e)
  {
    std::cerr << e.what() << std::endl
              << std::endl
              << parser.help();
    return -1;
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: '" << e.what() << "'" << std::endl;
    return -1;
  }
}
