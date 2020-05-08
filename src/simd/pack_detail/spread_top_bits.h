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

#ifndef SIMD_PACK_DETAIL_SPREAD_TOP_BITS_H_
#define SIMD_PACK_DETAIL_SPREAD_TOP_BITS_H_

#include "simd/pack_detail/pack_declaration.h"
#include "simd/pack_detail/top_bits.h"

namespace simd {
namespace _spread_top_bits {

// Based on https://stackoverflow.com/a/24242696/5021064
// This one also has information: https://stackoverflow.com/a/36491672/5021064

inline __m128i spread_16_chars(std::uint16_t mmask) {
  const __m128i cast = _mm_cvtsi32_si128(mmask);

  const __m128i shuffle_mask = _mm_set_epi64x(0x0101010101010101, 0);
  const __m128i spread = _mm_shuffle_epi8(cast, shuffle_mask);

  const __m128i bits_for_bytes = _mm_set1_epi64x(0x8040201008040201);
  auto isolated = _mm_and_si128(spread, bits_for_bytes);

  return _mm_cmpeq_epi8(bits_for_bytes, isolated);
}

inline __m128i spread_8_shorts(std::uint16_t mmask) {
  // Mask repeated
  auto spread = _mm_set1_epi16(mmask);
  auto bits_for_bytes = _mm_set_epi16(1 << 14, 1 << 12, 1 << 10, 1 << 8, 1 << 6,
                                      1 << 4, 1 << 2, 1);
  auto isolated = _mm_and_si128(spread, bits_for_bytes);
  return _mm_cmpeq_epi16(bits_for_bytes, isolated);
}

inline __m256i spread_32_chars(std::uint32_t mmask) {
  // we only use the low 32bits of each lane
  __m256i xbcast = _mm256_set1_epi32(mmask);

  // Each byte gets the source byte containing the corresponding bit
  __m256i shufmask = _mm256_set_epi64x(0x0303030303030303, 0x0202020202020202,
                                       0x0101010101010101, 0x0000000000000000);
  __m256i shuf = _mm256_shuffle_epi8(xbcast, shufmask);

  // mark bits that are responsible for each byte
  __m256i bits_for_bytes = _mm256_set1_epi64x(0x8040201008040201);
  __m256i isolated = _mm256_and_si256(shuf, bits_for_bytes);

  // If a bit in a byte was selected, select the whole byte.
  return _mm256_cmpeq_epi8(bits_for_bytes, isolated);
}

inline __m256i spread_8_ints(std::uint32_t mmask) {
  auto spread = _mm256_set1_epi32(mmask);
  auto bits_for_bytes = _mm256_set_epi32(1 << 28, 1 << 24, 1 << 20, 1 << 16,
                                         1 << 12, 1 << 8, 1 << 4, 1);
  auto isolated = _mm256_and_si256(spread, bits_for_bytes);
  return _mm256_cmpeq_epi32(bits_for_bytes, isolated);
}

}  // namespace _spread_top_bits

template <typename Pack>
Pack spread_top_bits(top_bits<Pack> mmask) {
  using reg_t = register_t<Pack>;
  using scalar = scalar_t<Pack>;

  if constexpr (mm::bit_width<reg_t>() == 128) {
    const std::uint16_t mmask_raw = static_cast<std::uint16_t>(mmask.raw);
    if constexpr (sizeof(scalar) >= 2) {
      return Pack{_spread_top_bits::spread_8_shorts(mmask_raw)};
    } else {
      return Pack{_spread_top_bits::spread_16_chars(mmask_raw)};
    }
  } else {
    if constexpr (sizeof(scalar) >= 4) {
      return Pack{_spread_top_bits::spread_8_ints(mmask.raw)};
    } else {
      return Pack{_spread_top_bits::spread_32_chars(mmask.raw)};
    }
  }
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_SPREAD_TOP_BITS_H_
