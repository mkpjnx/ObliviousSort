#pragma once
#include<cstdint>

namespace libUtil
{
  using label_t = uint64_t;

  template <typename T>
  struct Labeled {
    label_t label;
    T elem;
    bool real;
  };

} // namespace libUtil
