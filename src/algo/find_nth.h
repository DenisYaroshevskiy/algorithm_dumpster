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

#ifndef ALGO_FIND_NTH_H
#define ALGO_FIND_NTH_H

#include "algo/type_functions.h"

namespace algo {

template <typename I, typename N, typename P>
// require ForwardIterator<I> && Integral<N> && UnaryPredicate<ValueType<I>>
I find_nth_if_guarantied(I f, N n, P p) {
  I res = f;

  while (n >= 0) {
    while (!p(*f)) ++f;
    res = f;
    ++f;
    --n;
  }

  return res;
}

template <typename I, typename N, typename V>
// require ForwardIterator<I> && Integral<N> &&
//         EqualityComarable<ValueType<I>, V>
I find_nth_guarantied(I f, N n, const V& v) {
  return find_nth_if_guarantied(f, n, [&](Reference<I> x) { return x == v; });
}

}  // namespace algo

#endif  // ALGO_FIND_NTH_H
