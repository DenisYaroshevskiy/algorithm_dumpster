#ifndef ALGORITHM_FIND_NTH_H
#define ALGORITHM_FIND_NTH_H

#include "algorithm/type_functions.h"

namespace tools {

template <typename I, typename N, typename P>
I find_nth_if_guarantied(I f, N n, P p) {
  I res = f;

  while (n >= 0) {
    while (!p(*f)) ++f;
    res = f;
    ++f;
    --n;
  }

  return res;
}

template <typename I, typename N, typename V>
I find_nth_guarantied(I f, N n, const V& v) {
  return find_nth_if_guarantied(f, n, [&](Reference<I> x) { return x == v; });
}

}  // namespace tools

#endif  // ALGORITHM_FIND_NTH_H