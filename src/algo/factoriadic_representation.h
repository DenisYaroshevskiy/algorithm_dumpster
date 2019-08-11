/*
 * Copyright 2019 Denis Yaroshevskiy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ALGO_FACTORIADIC_REPRESENTATION_H
#define ALGO_FACTORIADIC_REPRESENTATION_H

#include <cstddef>
#include <numeric>
#include <utility>

#include "algo/type_functions.h"

namespace algo {

template <typename ResultN = std::ptrdiff_t, typename DigitType = ResultN,
          typename N = ResultN>
// require Integral<ResultN> && Integral<DigitType> && Integral<N>
constexpr ResultN compute_factoriadic_representation_length(N n) {
  ResultN res{1};

  for (DigitType digit{2}; n; ++digit) {
    ++res;
    n /= digit;
  }

  return res;
}

template <typename N, typename I>
// require Integral<N> && InputIterator<I>
constexpr N from_factoriadic_representation(I f, I l) {
  if (f == l) {
    return N{0};
  }

  ValueType<I> next_multiple{1};
  N factorial{1};

  ++f;  // skipping 0

  // std::accumulate is still not constexpr in my standard library
  N sum{0};
  while (f != l) {
    factorial *= next_multiple++;
    sum += factorial * *f;
    ++f;
  }

  return sum;
}

template <typename N, typename I>
// require Integral<N> && OutpuIterator<O>
constexpr I to_factoriadic_representation(N n, I o) {
  ValueType<I> digit{1};

  do {
    *o++ = static_cast<ValueType<I>>(n % digit);
    n /= digit;
    ++digit;
  } while (n);

  return o;
}

}  // namespace algo

#endif  // ALGO_FACTORIADIC_REPRESENTATION_H
