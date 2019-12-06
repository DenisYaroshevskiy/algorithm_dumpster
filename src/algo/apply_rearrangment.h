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

#include <iostream>

#include "algo/type_functions.h"

namespace algo {
namespace detail {

template <typename II>
// require RandomAccessIterator<II> && Position<ValueType<II>
constexpr void cycle_from_position(II f, II cur, ValueType<II> base,
                                   ValueType<II> marker) {
  using N = DifferenceType<II>;
  using I = ValueType<II>;
  using T = ValueType<I>;

  const N start = cur - f;
  N next_n = N{*cur - base};

  if (next_n == start) return;

  T tmp = std::move(**cur);

  do {
    II next_cur = f + next_n;
    **cur = std::move(**next_cur);
    *cur = marker;
    cur = next_cur;
    next_n = N{*cur - base};
  } while (next_n != start);

  **cur = std::move(tmp);
  *cur = marker;
}

template <typename II>
// require RandomAccessIterator<II> && Position<ValueType<II>
constexpr void cycle_from_position_no_marker(II f, II cur, ValueType<II> base) {
  using N = DifferenceType<II>;
  using I = ValueType<II>;
  using T = ValueType<I>;

  const N start = cur - f;
  N next_n = N{*cur - base};

  if (next_n == start) return;

  T tmp = std::move(**cur);
  I marker;

  do {
    II next_cur = f + next_n;
    **cur = std::move(**next_cur);
    std::swap(*cur, marker);
    cur = next_cur;
    next_n = N{*cur - base};
  } while (next_n != start);

  **cur = std::move(tmp);
  *cur = marker;
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

template <typename II>
// require RandomAccessIterator<II> && Position<ValueType<II>
constexpr void apply_rearrangment(II f, II l, ValueType<II> base,
                                  ValueType<II> marker) {
  // precondition: positions from a range that f to l inducates
  //               are a permutation of a position sequence.
  //               base is the first position in that sequence.
  //               find(f, l, marker) == l
  II cur = f;
  while (cur != l) {
    detail::cycle_from_position(f, cur, base, marker);
    cur = std::find_if(++cur, l, [&](Reference<II> x) { return x != marker; });
  }
}

template <typename II>
// require RandomAccessIterator<II> && Position<ValueType<II>
constexpr void apply_rearrangment_no_marker(II f, II l, ValueType<II> base) {
  II cur = f;
  while (cur != l) {
    detail::cycle_from_position_no_marker(f, cur, base);
    ++cur;
  }
}

}  // namespace algo

#endif  // ALGO_APPLY_REARRANGEMENT_H
