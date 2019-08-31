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

#ifndef ALGO_COMPARISONS_H
#define ALGO_COMPARISONS_H

namespace algo {

struct less_by_first {
  template <typename T, typename U>
  constexpr bool operator()(const T& x, const U& y) const {
    return x.first < y.first;
  }
};

struct dereference_less {
  template <typename I>
  constexpr bool operator()(const I& x, const I& y) {
    return *x < *y;
  }
};

}  // namespace algo

#endif  // ALGO_COMPARISONS_H
