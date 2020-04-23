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

#include <immintrin.h>

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

inline std::pair<__m128i, std::uint8_t> mask128_epi8(
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

inline std::pair<__m128i, std::uint8_t> mask128_epi16(
    std::uint16_t mmask) {
  const std::uint64_t mmask_expanded =
      _pdep_u64(mmask, 0x1111111111111111) * 0xf;

  const std::uint8_t offset = static_cast<std::uint8_t>(_mm_popcnt_u32(mmask));

  const std::uint64_t compressed_idxes =
      _pext_u64(0xfedcba9876543210, mmask_expanded);

  const __m128i as_lower_8byte =
      _mm_cvtsi64_si128(compressed_idxes);
  const __m128i as_16bit = _mm_cvtepu8_epi16(as_lower_8byte);
  const __m128i shift_by_4 = _mm_slli_epi16(as_16bit, 4);
  const __m128i combined = _mm_or_si128(shift_by_4, as_16bit);
  const __m128i filter = _mm_set1_epi16(0x0f0f);
  const __m128i res = _mm_and_si128(combined, filter);

  return {res, offset};
}

inline std::pair<__m256i, std::uint8_t> mask256_epi32(
    std::uint32_t mmask) {
  const std::uint64_t mmask_expanded =
      _pdep_u64(mmask, 0x5555'5555'5555'5555) * 3;

  const std::uint8_t offset = static_cast<std::uint8_t>(_mm_popcnt_u32(mmask));

  const std::uint64_t compressed_idxes =
      _pext_u64(0x0706050403020100, mmask_expanded);

  const __m128i as_lower_8byte =
      _mm_cvtsi64_si128(compressed_idxes);
  const __m256i expanded = _mm256_cvtepu8_epi32(as_lower_8byte);
  return {expanded, offset};
}

}  // namespace _compress_mask

template <typename T>
std::pair<__m128i, std::uint8_t> compress_mask_for_shuffle_epi8(
    std::uint32_t mmask) {

  auto res = [&]{
    if constexpr (sizeof(T) == 1) {
      return _compress_mask::mask128_epi8(mmask);
    } else {
      return _compress_mask::mask128_epi16(mmask);
    }
  }();

  res.second /= sizeof(T);
  return res;
}

template <typename T>
std::pair<__m256i, std::uint8_t> compress_mask_for_permutevar8x32(
    std::uint32_t mmask) {
  static_assert(sizeof(T) >= 4);
  auto res = _compress_mask::mask256_epi32(mmask);
  res.second /= sizeof(T);
  return res;
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_COMPRESS_MASK_H_
