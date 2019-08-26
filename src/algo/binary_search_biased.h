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

#ifndef ALGO_BINARY_SEARCH_BIASED_H
#define ALGO_BINARY_SEARCH_BIASED_H

#include <algorithm>
#include <functional>
#include <iterator>

#include "algo/advance_up_to.h"
#include "algo/binary_search.h"
#include "algo/half_positive.h"

namespace algo {
namespace detail {

template <typename I, typename P>
I partition_point_biased_no_checks(I f, P p) {
  while (true) {
    // clang-format off
    if (!p(*f)) return f; ++f;
    if (!p(*f)) return f; ++f;
    if (!p(*f)) return f; ++f;
    // clang-format on
    for (DifferenceType<I> step = 2;; step += step) {
      I test = std::next(f, step);
      if (!p(*test)) break;
      f = ++test;
    }
  }
}

}  // namespace detail

template <typename I, typename P>
// require ForwardIterator<I> && UnaryPredicate<P, ValueType<I>>
constexpr I partition_point_biased_expensive_pred(I f, I l, P p) {
  DifferenceType<I> step = 1;
  while (true) {
    I test = f;
    DifferenceType<I> try_to_step = step;
    algo::advance_up_to(test, l, try_to_step);

    if (test == l || !p(*test))
      return partition_point_n(f, step - try_to_step, p);

    f = ++test;
    step += step;
  }
}

template <typename I, typename P>
constexpr I partition_point_biased(I f, I l, P p, std::forward_iterator_tag) {
  return partition_point_biased_expensive_pred(f, l, p);
}

template <typename I, typename P>
// requires ForwardIterator<I> && Predicate<P(ValueType<I>)>
constexpr I partition_point_biased(I f, I l, P p,
                                   std::random_access_iterator_tag) {
  DifferenceType<I> n = l - f;
  while (f != l) {
    DifferenceType<I> n2 = half_positive(n);
    I sent = f + n2;
    if (!p(*sent)) return detail::partition_point_biased_no_checks(f, p);
    f = ++sent;
    n -= n2 + 1;
  }
  return f;
}

template <typename I, typename P>
// requires ForwardIterator<I> && Predicate<P(ValueType<I>)>
constexpr I partition_point_biased(I f, I l, P p) {
  return partition_point_biased(f, l, p, IteratorCategory<I>{});
}

template <typename I, typename P>
// requires BidirectionalIterator<I> && UnaryPredicate<P(ValueType<I>)>
I partition_point_hinted(I f, I h, I l, P p) {
  I fwd_attempt = partition_point_biased(h, l, p);
  if (fwd_attempt != h) return fwd_attempt;
  return partition_point_biased(std::reverse_iterator<I>(h),
                                std::reverse_iterator<I>(f),
                                [&](Reference<I> x) { return !p(x); })
      .base();
}

template <typename I, typename V, typename Comp>
// require ForwardIterator<I> && StrictWeakOrdering<Comp, ValueType<I>, V>
constexpr I lower_bound_biased_expensive_cmp(I f, I l, const V& v, Comp comp) {
  return algo::partition_point_biased_expensive_pred(
      f, l, [&](Reference<I> x) { return comp(x, v); });
}

template <typename I, typename V>
// require ForwardIterator<I> && TotallyOrdered<ValueType<I>, V>
constexpr I lower_bound_biased_expensive_cmp(I f, I l, const V& v) {
  return algo::lower_bound_biased_expensive_cmp(f, l, v, std::less<>{});
}

template <typename I, typename V, typename Comp>
// require ForwardIterator<I> && StrictWeakOrdering<Comp, ValueType<I>, V>
I lower_bound_biased(I f, I l, const V& v, Comp comp) {
  return partition_point_biased(f, l,
                                [&](Reference<I> x) { return comp(x, v); });
}

template <typename I, typename V>
// require ForwardIterator<I> && TotallyOrdered<ValueType<I>, V>
I lower_bound_biased(I f, I l, const V& v) {
  return lower_bound_biased(f, l, v, std::less<>{});
}

template <typename I, typename V, typename Comp>
// requires BidirectionalIterator<I> && WeakComarable<ValueType<I>, V>
I lower_bound_hinted(I f, I h, I l, const V& v, Comp comp) {
  return partition_point_hinted(f, h, l,
                                [&](Reference<I> x) { return comp(x, v); });
}

template <typename I, typename V>
// requires BidirectionalIterator<I> && WeakComarable<ValueType<I>, V>
I lower_bound_hinted(I f, I h, I l, const V& v) {
  return lower_bound_hinted(f, h, l, v, std::less<>{});
}

}  // namespace algo

#endif  // ALGO_BINARY_SEARCH_BIASED_H
