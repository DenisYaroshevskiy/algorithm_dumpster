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

#ifndef SIMD_PACK_DETAIL_COMPARISONS_H
#define SIMD_PACK_DETAIL_COMPARISONS_H

#include "simd/bits.h"
#include "simd/pack_detail/comparisons_pairwise.h"
#include "simd/pack_detail/masks.h"
#include "simd/pack_detail/minmax_pairwise.h"
#include "simd/pack_detail/pack_cast.h"
#include "simd/pack_detail/pack_declaration.h"
#include "simd/pack_detail/vbool_tests.h"

namespace simd {
namespace _comparisons {

template <std::size_t W>
std::uint32_t movemask(const pack<std::uint8_t, W>& x) {
  return static_cast<std::uint32_t>(mm::movemask<std::uint8_t>(x.reg));
}

}  // namespace _comparisons

template <typename T, size_t W>
bool equal_full(const pack<T, W>& x, const pack<T, W>& y) {
  // For equality (for integers) we are OK to use bitwise equality.
  // We could also use memcmp, on clang that produced ~the same code.
  // However on gcc it didn't - I decided against it.

  const auto x_bytes = cast_to_bytes(x);
  const auto y_bytes = cast_to_bytes(y);

  const auto eq_bytes = equal_pairwise(x_bytes, y_bytes);
  return all_true(eq_bytes);
}

template <typename T, std::size_t W>
bool less_lexicographical(const pack<T, W>& x, const pack<T, W>& y) {
  // The trick is: compare both with mininum.
  // If x compares equal to the left of y we return true.

  const pack<T, W> mins = min_pairwise(x, y);
  const auto x_cmp = equal_pairwise(x, mins);
  const auto y_cmp = equal_pairwise(y, mins);

  // I'm good doing this with bytes, since for not bytes
  // we get more bytes.
  // FFFFF0000 and FF00 would both compare the same if comparing bytes.

  const std::uint32_t x_mmask = _comparisons::movemask(cast_to_bytes(x_cmp));
  const std::uint32_t y_mmask = _comparisons::movemask(cast_to_bytes(y_cmp));

  // Since the bits are written lsb for the most left one,
  // we need to compare appropriately.
  return lsb_less(y_mmask, x_mmask);
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_COMPARISONS_H
