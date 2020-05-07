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
// require ContigiousIterator<I> && VectorCheck<P, ValueType<I>>
P iteration_aligned_unguarded(I _f, P p) {
  using T = equivalent<ValueType<I>>;
  using pack = simd::pack<T, width>;
  using vbool = simd::vbool_t<pack>;

  auto* f = unsq::drill_down(_f);
  auto* aligned_f = simd::previous_aligned_address<pack>(f);

  auto cur = simd::load<pack>(aligned_f);

  // Deal with first bit, maybe not fully in the data
  {
    const std::uint32_t offset = static_cast<std::uint32_t>(f - aligned_f);
    auto ignore = simd::ignore_first_n_mask<vbool>(offset);
    if (p(aligned_f, cur, ignore)) return p;
  }

  // After that no offset checks for us.
  do {
    aligned_f += width;
    cur = simd::load<pack>(aligned_f);
  } while (!p(aligned_f, cur));

  return p;
}

template <std::size_t width, typename I, typename P>
// require ContigiousIterator<I> && VectorCheck<P, ValueType<I>>
P iteration_aligned(I _f, I _l, P p) {
  using pack = simd::pack<equivalent<ValueType<I>>, width>;
  using vbool = simd::vbool_t<pack>;

  // Not necessary but prevents a weird call with ignore everything
  if (_f == _l) return p;


  auto* l = unsq::drill_down(_l);

  auto loop_body = [&](auto* from, const pack& read, auto... ignore) {
    if (l - from < static_cast<std::ptrdiff_t>(width)) {
      auto mask = simd::ignore_last_n_mask<vbool>(from + width - l);
      mask = combine_ignore(mask, ignore...);
      p(from, read, mask);
      return true;
    }
    return p(from, read, ignore...);
  };

  iteration_aligned_unguarded<width>(_f, loop_body);
  return p;
}

}  // namespace unsq

#endif  // UNSQ_ITERATION_H_
