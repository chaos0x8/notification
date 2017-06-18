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

#include <vector>
#include <type_traits>
#include <cstring>

struct Raw
{
  template <class T, typename std::enable_if<std::is_pod<T>::value, int>::type = 0>
  void insert(const T* payload)
  {
    const auto prevSize = _raw.size();
    _raw.resize(prevSize + sizeof(*payload));
    std::memcpy(_raw.data()+prevSize, payload, sizeof(*payload));
  }

  void insert(const void* payload, size_t payloadSize)
  {
    const auto prevSize = _raw.size();
    _raw.resize(prevSize + payloadSize);
    std::memcpy(_raw.data()+prevSize, payload, payloadSize);
  }

  const unsigned char* data() const
  {
    return _raw.data();
  }

  size_t size() const
  {
    return _raw.size();
  }

private:
  std::vector<unsigned char> _raw;
};
