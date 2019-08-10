#ifndef ALGORITHM_TO_FACTORIADIC_REPRESENTATION_H
#define ALGORITHM_TO_FACTORIADIC_REPRESENTATION_H

#include <cstddef>

#include <iostream>

#include "type_functions.h"

namespace tools {

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

#endif  // ALGORITHM_TO_FACTORIADIC_REPRESENTATION_H