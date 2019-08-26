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

#ifndef ALGO_ADVANCE_UP_TO_H
#define ALGO_ADVANCE_UP_TO_H

#include <algorithm>

#include "algo/type_functions.h"

namespace algo {

template <typename I>
// require InputIterator<I>
constexpr void advance_up_to(I& f, I l, DifferenceType<I>& n) {
  if constexpr (RandomAccessIterator<I>) {
    DifferenceType<I> actual_n = std::min(n, l - f);
    f += actual_n;
    n -= actual_n;
  } else {
    while (f != l && n) {
      ++f;
      --n;
    }
  }
}

}  // namespace algo

#endif  // ALGO_ADVANCE_UP_TO_H
