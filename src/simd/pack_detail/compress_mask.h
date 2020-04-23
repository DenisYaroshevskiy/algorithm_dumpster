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

#ifndef SIMD_PACK_DETAIL_COMPRESS_MASK_H_
#define SIMD_PACK_DETAIL_COMPRESS_MASK_H_

#include "simd/pack_detail/pack_declaration.h"
#include "simd/pack_detail/top_bits.h"

#include <algorithm>
#include <tuple>
#include <type_traits>

#include <iostream>

namespace simd {
namespace _compress_mask {

// Based on: https://stackoverflow.com/a/36951611/5021064
// Mask functions return the compressed indexes and the popcount.

// all_indexes - indexes either from 0 to 7 or from 8 to f
inline std::pair<std::uint64_t, std::uint8_t> mask64_epi8(
    std::uint8_t mmask, std::uint64_t all_idxes) {
  const std::uint64_t mmask_expanded =
      _pdep_u64(mmask, 0x0101010101010101) * 0xff;

  const std::uint8_t offset = static_cast<std::uint8_t>(_mm_popcnt_u32(mmask));

  const std::uint64_t compressed_idxs = _pext_u64(all_idxes, mmask_expanded);

  return {compressed_idxs, offset};
}

inline std::pair<mm::register_i<128>, std::uint8_t> mask128_epi8(
    std::uint16_t mmask) {
  auto [first_mask, first_n] = mask64_epi8(mmask & 0xff, 0x0706050403020100);
  auto [second_mask, second_n] = mask64_epi8(mmask >> 8, 0x0f0e0d0c0b0a0908);

  first_mask |= first_n == 8 ? 0 : second_mask << (first_n * 8);
  second_mask = first_n == 0 ? 0 : second_mask >> ((8 - first_n) * 8);

  return {_mm_set_epi64x(second_mask, first_mask), first_n + second_n};
}

/*
  For uint16_t, element byte indexes
  0 => 0x0100, 256
  1 => 0x0302, 770
  2 => 0x0504, 1284
  3 => 0x0706, 1798
  4 => 0x0908, 2312
  5 => 0x0b0a, 2826
  6 => 0x0d0c, 3340
  7 => 0x0f0e, 3854

We can write 0xfe into the 7th
Then to get => 0x0f0e:
 (x << 4) | x & 0x0f0f
 */

inline std::pair<mm::register_i<128>, std::uint8_t> mask128_epi16(
    std::uint16_t mmask) {
  const std::uint64_t mmask_expanded =
      _pdep_u64(mmask, 0x1111111111111111) * 0xf;

  const std::uint8_t offset = static_cast<std::uint8_t>(_mm_popcnt_u32(mmask));

  const std::uint64_t compressed_idxes =
      _pext_u64(0xfedcba9876543210, mmask_expanded);

  const mm::register_i<128> as_lower_8byte = _mm_cvtsi64_si128(compressed_idxes);
  const mm::register_i<128> as_16bit = _mm_cvtepu8_epi16(as_lower_8byte);
  const mm::register_i<128> shift_by_4 = _mm_slli_epi16(as_16bit, 4);
  const mm::register_i<128> combined = _mm_or_si128(shift_by_4, as_16bit);
  const mm::register_i<128> filter = _mm_set1_epi16(0x0f0f);
  const mm::register_i<128> res = _mm_and_si128(combined, filter);

  return {res, offset};
}

}  // namespace _compress_mask

template <typename T, std::size_t W>
constexpr bool compress_mask_is_supported() {
  return W * sizeof(T) == 16;
}

template <typename T, std::size_t W>
inline std::pair<pack<T, W>, std::uint8_t> compress_mask(
    top_bits<pack<T, W>> mmask) {
  static_assert(compress_mask_is_supported<T, W>());

  auto [mm_compressed, count] = [&] {
    if constexpr(sizeof(T) >= 2) {
      return _compress_mask::mask128_epi16(static_cast<std::uint16_t>(mmask.raw));
    } else {
      return _compress_mask::mask128_epi8(static_cast<std::uint16_t>(mmask.raw));
    }
  }();

  count /= sizeof(T);
  return {pack<T, W>{mm_compressed}, count};
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_COMPRESS_MASK_H_
