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

#ifndef ALGO_STABLE_SORT_H
#define ALGO_STABLE_SORT_H

#include <iterator>
#include <type_traits>
#include <utility>
#include <vector>

#include "algo/half_positive.h"
#include "algo/merge.h"
#include "algo/move.h"
#include "algo/type_functions.h"

namespace algo {

template <typename I, typename N, typename B, typename R>
// require BiderectionalIterator<I> && Number<N> && ForwardIterator<B>
//         && WeakStrictOrdering<R, ValueType<I>>
I stable_sort_n_buffered(I f, N n, R r, B buf) {
  if (!n) return f;
  if (n == N(1)) return ++f;

  N half = algo::half_positive(n);
  auto [m, buf_l] = algo::move_n(f, half, buf);

  stable_sort_n_buffered(buf, half, r, f);
  I l = stable_sort_n_buffered(m, n - half, r, f);

  using MI = std::move_iterator<I>;
  using MB = std::move_iterator<B>;
  return algo::merge(MB(buf), MB(buf_l), MI(m), MI(l), f, r);
}

template <typename I, typename N, typename R>
// require BiderectionalIterator<I> && Number<N> && ForwardIterator<B>
//         && WeakStrictOrdering<R, ValueType<I>>
I stable_sort_n_sufficient_allocation(I f, N n, R r) {
  std::vector<ValueType<I>> buf(algo::half_positive(n));
  return algo::stable_sort_n_buffered(f, n, r, buf.begin());
}

template <typename I, typename N, typename R>
I stable_sort_n_sufficient_allocation(I f, N n) {
  return stable_sort_n_sufficient_allocation(f, n, std::less<>{});
}

template <typename I, typename R>
void stable_sort_sufficient_allocation(I f, I l, R r) {
  stable_sort_n_sufficient_allocation(f, std::distance(f, l), r);
}

template <typename I, typename R>
void stable_sort_sufficient_allocation(I f, I l) {
  stable_sort_sufficient_allocation(f, l, std::less<>{});
}

}  // namespace algo

#endif  // ALGO_STABLE_SORT_H
