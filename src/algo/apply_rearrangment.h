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

#ifndef ALGO_APPLY_REARRANGEMENT_H
#define ALGO_APPLY_REARRANGEMENT_H

#include <algorithm>
#include <type_traits>

#include "algo/type_functions.h"

namespace algo {
namespace detail {

template <typename II, typename Op>
// require RandomAccessPositionIteator<II> &&
//         IteratorToPosition<Op, ValueType<I>>
constexpr void cycle_from_position(II f, II cur, Op original_pos, ValueType<II> marker) {
  using N = DifferenceType<II>;

  const N start = cur - f;
  N next_n = N{original_pos(*cur)};

  if (next_n == start) return;

  ValueType<ValueType<II>> tmp = std::move(**cur);

  do {
    II next_cur = f + next_n;
    **cur = std::move(**next_cur);
    *cur = marker;
    cur = next_cur;
    next_n = N{original_pos(*cur)};
  } while (next_n != start);

  **cur = std::move(tmp);
}

}  // namespace detail

template <typename II, typename O>
// require ForwardPositionIterator<II> && OutputIterator<O> &&
//         std::is_same_v<ValueType<O>, ValueType<ValueType<II>>
constexpr void apply_rearrangment_copy(II f, II l, O o) {
  while (f != l) {
    *o = **f;
    ++o;
    ++f;
  }
}

template <typename II, typename O>
// require ForwardPositionIterator<II> && OutputIterator<O> &&
//         std::is_same_v<ValueType<O>, ValueType<ValueType<II>>
constexpr void apply_rearrangment_move(II f, II l, O o) {
  while (f != l) {
    *o = std::move(**f);
    ++o;
    ++f;
  }
}

template <typename II, typename Op>
// require RandomAccessPositionIteator<II> &&
//         IteratorToPosition<Op, ValueType<I>>
auto apply_rearrangment(II f, II l, Op original_pos, ValueType<II> marker)
    -> std::enable_if_t<std::is_invocable_v<Op, ValueType<II>>> {
  // precondition: iterators from *f to *l are a permutation
  //               of an increasing iterator sequence
  //               and pos returns number of an element in that
  //               orignal sequence.
  II cur = f;
  while (cur != l) {
    detail::cycle_from_position(f, cur, original_pos, marker);
    cur = std::find_if(++cur, l, [&](Reference<II> x) { x != marker; });
  }
}

template <typename II>
// require RandomAccessPositionIteator<II> &&
//         ForwardNIterator<ValueType<II>
void apply_rearrangment(II f, II l, ValueType<II> base, ValueType<II> marker) {
  apply_rearrangment(f, l, [&](ValueType<II> x) { return x - base; }, marker);
}

}  // namespace algo

#endif  // ALGO_APPLY_REARRANGEMENT_H
