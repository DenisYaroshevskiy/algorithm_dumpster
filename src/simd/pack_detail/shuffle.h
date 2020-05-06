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

#ifndef SIMD_PACK_DETAIL_SHUFFLE_H
#define SIMD_PACK_DETAIL_SHUFFLE_H

#include <array>

#include "simd/pack_detail/pack_declaration.h"

namespace simd {
namespace _shuffle {

inline mm::register_i<128> swap_adjacent_16_bytes_mask() {
  return _mm_set_epi8(14, 15, 12, 13, 10, 11, 8, 9, 6, 7, 4, 5, 2, 3, 0, 1);
}

inline mm::register_i<256> swap_adjacent_32_bytes_mask() {
  return _mm256_set_m128(swap_adjacent_16_bytes_mask(),
                         swap_adjacent_16_bytes_mask());
}

template <std::size_t byte_width, typename Register>
Register swap_adjacent(Register x) {
  static constexpr auto two_elements_4_parts_shuffle = _MM_SHUFFLE(1, 0, 3, 2);
  static constexpr auto four_element_shuffle = _MM_SHUFFLE(2, 3, 0, 1);

  if constexpr (byte_width == 1 && mm::bit_width<Register>() == 128) {
    return _mm_shuffle_epi8(x, swap_adjacent_16_bytes_mask());
  } else if constexpr (byte_width == 1 && mm::bit_width<Register>() == 256) {
    return _mm256_shuffle_epi8(x, swap_adjacent_32_bytes_mask());
  } else if constexpr (byte_width == 2 && mm::bit_width<Register>() == 128) {
    x = _mm_shufflehi_epi16(x, four_element_shuffle);
    return _mm_shufflelo_epi16(x, four_element_shuffle);
  } else if constexpr (byte_width == 2 && mm::bit_width<Register>() == 256) {
    x = _mm256_shufflehi_epi16(x, four_element_shuffle);
    return _mm256_shufflelo_epi16(x, four_element_shuffle);
  } else if constexpr (byte_width == 4 && mm::bit_width<Register>() == 128) {
    return _mm_shuffle_epi32(x, four_element_shuffle);
  } else if constexpr (byte_width == 4 && mm::bit_width<Register>() == 256) {
    return _mm256_shuffle_epi32(x, four_element_shuffle);
  } else if constexpr (byte_width == 8 && mm::bit_width<Register>() == 128) {
    return _mm_shuffle_epi32(x, two_elements_4_parts_shuffle);
  } else if constexpr (byte_width == 8 && mm::bit_width<Register>() == 256) {
    return _mm256_permute4x64_epi64(x, four_element_shuffle);
  } else if constexpr (byte_width == 16 && mm::bit_width<Register>() == 256) {
    return _mm256_permute4x64_epi64(x, two_elements_4_parts_shuffle);
  } else {
    return error_t{};
  }
}

}  // namespace _shuffle

template <std::size_t group_size, typename T, std::size_t W>
pack<T, W> swap_adjacent_groups(const pack<T, W>& x) {
  return pack<T, W>{_shuffle::swap_adjacent<group_size * sizeof(T)>(x.reg)};
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_SHUFFLE_H
