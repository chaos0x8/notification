#pragma once

#include <cstring>
#include <type_traits>
#include <vector>

struct Raw {
  template <class T,
    typename std::enable_if<std::is_pod<T>::value, int>::type = 0>
  void insert(const T* payload) {
    const auto prevSize = _raw.size();
    _raw.resize(prevSize + sizeof(*payload));
    std::memcpy(_raw.data() + prevSize, payload, sizeof(*payload));
  }

  void insert(const void* payload, size_t payloadSize) {
    const auto prevSize = _raw.size();
    _raw.resize(prevSize + payloadSize);
    std::memcpy(_raw.data() + prevSize, payload, payloadSize);
  }

  const unsigned char* data() const {
    return _raw.data();
  }

  size_t size() const {
    return _raw.size();
  }

private:
  std::vector<unsigned char> _raw;
};
