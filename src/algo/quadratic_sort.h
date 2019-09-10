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

#include "algo/type_functions.h"

namespace algo {
namespace detail {

template <typename I, typename R>
// require BidirectionalIterator<I> && WeakStrictOrdering<R, ValueType<I>>
I linear_insert(I f, I cur, R r) {
  // f != cur
  I before = cur;
  --before;
  if (!r(*cur, *before)) return cur;

  ValueType<I> tmp = std::move(*cur);

  while(true) {
    I write_to = cur;
    cur = before;
    *write_to = std::move(*cur);
    if (f == cur) break;
    --before;
    if (!r(tmp, *before)) break;
  }

  *cur = std::move(tmp);
  return cur;
}

}  // namespace detail

template <typename I, typename N, typename R>
// require ForwardIterator<I> && Number<N> &&
//         WeakStrictOrdering<R, ValueType<I>>
void bubble_sort_n(I f, N n, R r) {
  while (--n > 0) {
    N m = n;
    I cur = f;
    do {
      I next = std::next(cur);
      if (r(*next, *cur)) std::iter_swap(cur, next);
      cur = next;
    } while (--m);
  }
}

template <typename I, typename R>
// require BidirectionalIterator<I> && WeakStrictOrdering<R, ValueType<I>>
I insertion_sort_n(I f, DifferenceType<I> n, R r) {
  if (n == 0) return f;

  I cur = f;

  while (true) {
    --n;
    ++cur;
    if (!n) break;
    detail::linear_insert(f, cur, r);
  }

  return cur;
}

template <typename I, typename R>
// require ForwardIterator<I> && WeakStrictOrdering<R, ValueType<I>>
I quadratic_sort_n(I f, DifferenceType<I> n, R r) {
  if constexpr (BidirectionalIterator<I>) {
    return insertion_sort_n(f, n, r);
  } else {
    bubble_sort_n(f, n, r);
    return std::next(f, n);
  }
}

}  // namespace algo

#endif  // ALGO_QUADRATIC_SORT_H
