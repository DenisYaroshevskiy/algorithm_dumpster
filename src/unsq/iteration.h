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

#ifndef UNSQ_ITERATION_H_
#define UNSQ_ITERATION_H_

#include "simd/pack.h"
#include "unsq/drill_down.h"

namespace unsq {

template <std::size_t width, typename I, typename P>
// require ContigiousIterator<I> && Predictate<P, equivalent_iterator<I>, ...ignore>
P iteration_aligned_unguarded(I _f, P p) {
  using T = equivalent<ValueType<I>>;
  using pack = simd::pack<T, width>;
  using vbool = simd::vbool_t<pack>;

  auto* f = unsq::drill_down(_f);
  auto* aligned_f = simd::previous_aligned_address<pack>(f);

  // Deal with first bit, maybe not fully in the data
  {
    auto ignore = simd::ignore_first_n_mask<vbool>(f - aligned_f);
    if (p(aligned_f, ignore)) return p;
  }

  // After that no offset checks for us.
  do {
    aligned_f += width;
  } while (!p(aligned_f));

  return p;
}

// For iteration aligned have to copy paste code form unaligned
// Index operations didn't get optimized well.

template <std::size_t width, typename I, typename P>
// require ContigiousIterator<I> && Predictate<P, equivalent_iterator<I>, ...ignore>
P iteration_aligned(I _f, I _l, P p) {
  using pack = simd::pack<equivalent<ValueType<I>>, width>;
  using vbool = simd::vbool_t<pack>;

  // Have to do this, otherwise might be not able to load a single pack.
  if (_f == _l) return p;

  auto [f, l] = unsq::drill_down_range(_f, _l);
  auto* aligned_f = simd::previous_aligned_address<pack>(f);
  auto* aligned_l = simd::previous_aligned_address<pack>(l);

  // Deal with first bit, maybe not fully in the data
  auto ignore = simd::ignore_first_n_mask<vbool>(f - aligned_f);

  if (aligned_f != aligned_l) {
    // first bit check
    if (p(aligned_f, ignore)) return p;
    ignore = simd::ignore_first_n_mask<vbool>(0);

    // Go fully in range
    while (true) {
      aligned_f += width;
      if (aligned_f == aligned_l) {
        break;
      }
      if (p(aligned_f)) return p;
    }
    if (aligned_l == l) return p;
  }

  auto ignore_last = simd::ignore_last_n_mask<vbool>(aligned_l + width - l);
  ignore = combine_ignore(ignore, ignore_last);
  p(aligned_l, ignore);
  return p;
}

}  // namespace unsq

#endif  // UNSQ_ITERATION_H_
