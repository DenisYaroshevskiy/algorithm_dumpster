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

#include "algo/half_nonnegative.h"
#include "algo/type_functions.h"

namespace algo {

template <typename I, typename P>
// require ForwardIterator<I> && UnaryPredicate<P, ValueType<I>>
constexpr I partition_point_n(I f, DifferenceType<I> n, P p) {
  while (n) {
    DifferenceType<I> n2 = half_nonnegative(n);
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

template <typename I, typename P>
// require ForwardIterator<I> && UnaryPredicate<P, ValueType<I>>
constexpr I partition_point(I f, I l, P p) {
  return algo::partition_point_n(f, std::distance(f, l), p);
}

template <typename I, typename V, typename Comp>
// require ForwardIterator<I> && StrictWeakOrdering<Comp, ValueType<I>, V>
constexpr I lower_bound_n(I f, DifferenceType<I> n, const V& v, Comp comp) {
  return algo::partition_point_n(f, n, [&](Reference<I> x) { return comp(x, v); } );
}

template <typename I, typename V>
// require ForwardIterator<I> && TotallyOrdered<ValueType<I>, V>
constexpr I lower_bound_n(I f, DifferenceType<I> n, const V& v) {
  return algo::lower_bound_n(f, n, v, std::less<>{});
}

template <typename I, typename V, typename Comp>
// require ForwardIterator<I> && StrictWeakOrdering<Comp, ValueType<I>, V>
constexpr I lower_bound(I f, I l, const V& v, Comp comp) {
  return algo::partition_point(f, l, [&](Reference<I> x) { return comp(x, v); } );
}

template <typename I, typename V>
// require ForwardIterator<I> && TotallyOrdered<ValueType<I>, V>
constexpr I lower_bound(I f, I l, const V& v) {
  return algo::lower_bound(f, l, v, std::less<>{});
}

}  // namespace algo

#endif  // ALGO_BINARY_SEARCH_H
