/*
 * Copyright 2020 Denis Yaroshevskiy
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

#ifndef UNSQ_FIND_H_
#define UNSQ_FIND_H_

#include "simd/pack.h"
#include "unsq/drill_down.h"
#include "unsq/iteration.h"

namespace unsq {

template <std::size_t width, typename I, typename PV>
// require ContigiousIterator<I> && VectorPredicate<PV, equivalent<ValueType<I>>
I find_if_unguarded(I _f, PV p) {
  using T = equivalent<ValueType<I>>;
  using pack = simd::pack<T, width>;
  using vbool = simd::vbool_t<pack>;

  auto* f = unsq::drill_down(_f);
  auto* found = f;

  auto loop_body = [&](auto* from, const pack& read, auto... ignore) mutable {
    const vbool test = p(read);
    const simd::top_bits<vbool> mmask = simd::get_top_bits(test, ignore...);
    const std::optional match = simd::first_true(mmask);
    if (!match) return false;
    found = from + *match;
    return true;
  };

  iteration_aligned_unguarded<width>(f, loop_body);
  return undo_drill_down(_f, found);
}

template <std::size_t width, typename I, typename T>
// require ContigiousIterator<I> && Convertible<T, ValueType<P>>
I find_unguarded(I f, const T& x) {
  using U = equivalent<ValueType<I>>;
  using pack = simd::pack<U, width>;

  auto xs = simd::set_all<pack>((U)x);

  return unsq::find_if_unguarded<width>(
      f, [&](const pack& read) { return simd::equal_pairwise(read, xs); });
}

}  // namespace unsq

#endif  // UNSQ_FIND_H_
