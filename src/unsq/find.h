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
namespace _find {

template <std::size_t width, typename _I, typename PV>
// require ContigiousIterator<_I> && VectorPredicate<PV,
// equivalent<ValueType<I>>
struct find_if_body {
  using I = equivalent_iterator<_I>;
  using T = ValueType<I>;
  using pack = simd::pack<T, width>;
  using vbool = simd::vbool_t<pack>;

  PV p;
  I found;

  find_if_body(PV p, I not_found) : p{p}, found{not_found} {}

  template <typename... Ignore>
  bool operator()(I from, const pack& read, Ignore... ignore) {
    const vbool test = p(read);
    const simd::top_bits<vbool> mmask = simd::get_top_bits(test, ignore...);

    const std::optional match = simd::first_true(mmask);
    if (!match) return false;
    found = from + *match;
    return true;
  }
};

template <typename I, std::size_t width, typename T>
auto equal_to(const T& x) {
  using pack = simd::pack<equivalent<ValueType<I>>, width>;

  auto xs = simd::set_all<pack>(equivalent_cast(x));
  return [xs](const pack& read) { return simd::equal_pairwise(read, xs); };
}

}  // namespace _find

template <std::size_t width, typename I, typename PV>
// require ContigiousIterator<I> && VectorPredicate<PV, equivalent<ValueType<I>>
I find_if_unguarded(I f, PV p) {
  _find::find_if_body<width, I, PV> body{p, drill_down(f)};
  auto found = iteration_aligned_unguarded<width>(f, body).found;

  return undo_drill_down(f, found);
}

template <std::size_t width, typename I, typename T>
// require ContigiousIterator<I> && Convertible<T, ValueType<P>>
I find_unguarded(I f, const T& x) {
  return unsq::find_if_unguarded<width>(f, _find::equal_to<I, width>(x));
}

template <std::size_t width, typename I, typename PV>
// require ContigiousIterator<I> && VectorPredicate<PV, equivalent<ValueType<I>>
I find_if(I f, I l, PV p) {
  auto not_found = drill_down_range(f, l).second;

  _find::find_if_body<width, I, PV> body{p, not_found};
  auto found = iteration_aligned<width>(f, l, body).found;

  return undo_drill_down(f, found);
}

template <std::size_t width, typename I, typename T>
// require ContigiousIterator<I> && Convertible<T, ValueType<P>>
I find(I f, I l, const T& x) {
  return unsq::find_if<width>(f, l, _find::equal_to<I, width>(x));
}

}  // namespace unsq

#endif  // UNSQ_FIND_H_
