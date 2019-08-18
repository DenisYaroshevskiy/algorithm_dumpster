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

#ifndef ALGO_BINARY_SEARCH_H
#define ALGO_BINARY_SEARCH_H

#include <type_functions.h>

namespace algo {

template <typename I, typename P>
I partition_point_n(I f, DifferenceType<I> n, P p) {
  while (n) {
    DifferenceType<I> n2 = detail::half_positive(n);
    I m = std::next(f, n2);
    if (p(*m)) {
      f = ++m;
      n -= n2 + 1;
    } else {
      n = n2;
    }
  }
  return f;
}

}  // namespace algo

#endif  // ALGO_BINARY_SEARCH_H
