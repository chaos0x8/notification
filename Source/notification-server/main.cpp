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
#include "OptionParser.hpp"
#include <iostream>
#include <gtk/gtk.h>

enum class Tag : uint32_t
{
  Stop,
  Help
};

int main(int argc, char** argv)
{
  try
  {
    using namespace Common::OptionParser;

    gtk_init(&argc, &argv);

    auto opts = makeParser<Tag>(
      tagged<Tag, Tag::Stop>(Option<bool>("-s", "--stop").description("stops running server")),
      tagged<Tag, Tag::Help>(Option<bool>("-h", "--help").description("displays help"))
    );

    opts.addHelpPrefix("Usage: notification-server [options]");
    opts.parse(&argc, argv);

    auto args = Args(&argc, argv);

    if (opts.get<Tag::Help>().value())
    {
      std::cout << opts.help();
      return 0;
    }
    else if (opts.get<Tag::Stop>().value())
    {
      Application::stop();
      return 0;
    }
    else
    {
      return Application::run(args);
    }
  }
  catch (std::exception& e)
  {
    std::cerr << "Exception: '" << e.what() << "'" << std::endl;
    return -1;
  }
}
