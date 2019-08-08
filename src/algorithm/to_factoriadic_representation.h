#ifndef ALGORITHM_TO_FACTORIADIC_REPRESENTATION_H
#define ALGORITHM_TO_FACTORIADIC_REPRESENTATION_H

#include <cstddef>

#include <iostream>

#include "type_functions.h"

namespace tools {
namespace detail {

template <typename DigitType, typename N, typename Op>
constexpr void do_to_factoriadic_representation(N n, Op op) {
  DigitType digit(1);

  do {
    op(DigitType(n % digit));
    n /= digit;
    ++digit;
  } while (n);
}

}  // namespace detail

template <typename ResultN = std::ptrdiff_t, typename DigitType = ResultN,
          typename N = ResultN>
constexpr ResultN compute_factoriadic_representation_length(N n) {
  ResultN r{0};
  detail::do_to_factoriadic_representation<DigitType>(
      std::move(n), [&](const DigitType&) { ++r; });
  return r;
}

template <typename N, typename I>
constexpr I to_factoriadic_representation(N n, I o) {
  detail::do_to_factoriadic_representation<ValueType<I>>(
      std::move(n), [&](ValueType<I> x) { *o++ = std::move(x); });
  return o;
}

}  // namespace tools

#endif  // ALGORITHM_TO_FACTORIADIC_REPRESENTATION_H