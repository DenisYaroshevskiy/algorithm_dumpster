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

#ifndef UNSQ_REMOVE_H_
#define UNSQ_REMOVE_H_

#include "unsq/drill_down.h"
#include "simd/pack.h"

namespace unsq {
namespace _remove {

template <typename T, std::size_t W>
std::uint32_t movemask(const simd::pack<T, W>& x) {
  return static_cast<std::uint32_t>(mm::movemask<std::uint8_t>(x.reg));
}

// Figure out a safe load. (| - page boundary)
// [f  |  l]  => Ok from f, Ok from l - width
// [|f  l] => Ok from f, Not Ok from l - width
// [f, l|] => Not ok from f, Ok from l - width
template <std::size_t width, typename T>
std::pair<T*, std::uint32_t> figure_out_safe_load(T* f, T* l) {
  T* page_boundary = simd::end_of_page(f);

  if (page_boundary - f) {
    T* safe = l - width;
    std::uint32_t mmask_filter = ~simd::set_lower_n_bits((f - safe) * sizeof(T));
    return {safe, mmask_filter};
  }

  std::uint32_t mmask_filter = simd::set_lower_n_bits((l - f) * sizeof(T));
  return {f, mmask_filter};
}

}  // namespace _remove

template <std::size_t width, typename I, typename PV>
// require ContigiousIterator<I> && VectorPredicate<PV, equivalent<ValueType<I>>
I remove_if(I _f, I _l, PV p) {
  using T = equivalent<typename I::value_type>;
  auto [f, l] = unsq::drill_down_range(_f, _l);
  T* o = f;

  using pack = simd::pack<T, width>;
  using vbool = simd::vbool_t<pack>;

  // There is a tradeoff between doing aligned reads and more comresses
  // and unaligned with less compresses.
  // For now I do unaligned.

  auto get_mmask = [&](const pack& ts) {
    const vbool test = p(ts);
    return ~_remove::movemask(test); // p marks trues to remove
  };

  while ((l - f) >= static_cast<std::ptrdiff_t>(width))
  {
    const pack ts = simd::load_unaligned<pack>(f);
    const std::uint32_t mmask = get_mmask(ts);
    o = simd::compress_store_unsafe(o, ts, mmask);
    f += width;
  }

  auto [safe, mmask_filter] = _remove::figure_out_safe_load<width>(f, l);

  const pack ts = simd::load_unaligned<pack>(safe);
  std::uint32_t mmask = get_mmask(ts);
  mmask &= mmask_filter;

  o = simd::compress_store_masked(o, ts, mmask);
  return unsq::undo_drill_down(_f, o);
}

template <std::size_t width, typename I, typename T>
I remove(I f, I l, const T& x) {
  using U = equivalent<typename I::value_type>;
  using pack = simd::pack<U, width>;

  auto xs = simd::set_all<pack>((U)x);

  return unsq::remove_if<width>(f, l, [&](const pack& read) {
    return simd::equal_pairwise(read, xs);
  });
}

}  // namespace unsq

#endif  // UNSQ_REMOVE_H_
