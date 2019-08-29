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

#include "algo/copy.h"
#include "algo/half_positive.h"
#include "algo/type_functions.h"

namespace algo {

template <typename I, typename N, typename B, typename R>
// require BiderectionalIterator<I> && Number<N> && WeakStrictOrdering<R, ValueType<I>>
//         && ForwardIterator<B>
I stable_sort_n_buffered(I f, N n, R r, B buf) {
  if (!n) return;
  N half = algo::half_positive(n);
  auto [m, buf_l] = algo::copy_n(std::move_iterator<I>(f), half, buf);

  I m = stable_sort_n_buffered(f, half, r, buf);

}

}  // namespace algo

#endif  // ALGO_STABLE_SORT_H
