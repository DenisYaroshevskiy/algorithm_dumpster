#ifndef ALGORITHM_FACTORIADIC_REPRESENTATION_H
#define ALGORITHM_FACTORIADIC_REPRESENTATION_H

#include <cstddef>
#include <numeric>
#include <utility>

#include "type_functions.h"

namespace tools {

template <typename N, typename I>
constexpr N from_factoriadic_representation(I f, I l) {
  if (f == l) {
    return N{0};
  }

  ValueType<I> next_multiple{1};
  N factorial{1};

  // std::accumulate is still not constexpr in my standard library
  N sum{0};
  ++f;
  while (f != l) {
    factorial *= next_multiple++;
    sum += factorial * *f;
    ++f;
  }

  return sum;
}

template <typename ResultN = std::ptrdiff_t, typename DigitType = ResultN,
          typename N = ResultN>
constexpr ResultN compute_factoriadic_representation_length(N n) {
  ResultN res{1};

  for (DigitType digit{2}; n; ++digit) {
    ++res;
    n /= digit;
  }

  return res;
}

template <typename N, typename I>
constexpr I to_factoriadic_representation(N n, I o) {
  ValueType<I> digit{1};

  do {
    *o++ = static_cast<ValueType<I>>(n % digit);
    n /= digit;
    ++digit;
  } while (n);

  return o;
}

}  // namespace tools

#endif  // ALGORITHM_FACTORIADIC_REPRESENTATION_H