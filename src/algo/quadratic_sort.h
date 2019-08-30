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

#ifndef ALGO_QUADRATIC_SORT_H
#define ALGO_QUADRATIC_SORT_H

#include <algorithm>

namespace algo {

template <typename I, typename N, typename R>
// require ForwardIterator<I> && Number<N> && WeakStrictOrdering<R,
// ValueType<I>>
void bubble_sort_n(I f, N n, R r) {
  while (--n) {
    N m = n;
    I cur = f;
    do {
      I next = std::next(cur);
      if (r(*next, *cur)) std::iter_swap(cur, next);
      cur = next;
    } while (--m);
  }
}

}  // namespace algo

#endif  // ALGO_QUADRATIC_SORT_H
