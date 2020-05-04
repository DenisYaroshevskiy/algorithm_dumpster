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

#ifndef SIMD_PACK_DETAIL_REDUCE_H_
#define SIMD_PACK_DETAIL_REDUCE_H_

#include "simd/pack_detail/operators.h"
#include "simd/pack_detail/pack_declaration.h"

namespace simd {
namespace _reduce {

template <std::size_t byte_width, typename Register>
Register swap_adjacent(Register x) {
  static constexpr auto two_elements_4_parts_shuffle = _MM_SHUFFLE(1, 0, 3, 2);
  static constexpr auto four_element_shuffle = _MM_SHUFFLE(2, 3, 0, 1);

  if constexpr (byte_width == 2 && mm::bit_width<Register>() == 128) {
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

// Reduction for 4 numbers. We have to commute:
//
// [0, 1, 2, 3] + [2, 3, 0, 1] =
//    [0 + 2, 1 + 3, 2 + 0, 1 + 3]
// [0 + 2, 1 + 3, 2 + 0, 1 + 3] + [1 + 3, 0 + 2, 1 + 3, 0 + 2] =
//    [0 + 1 + 2 + 3, 0 + 1 + 2 + 3, 0 + 1 + 2 + 3, 0 + 1 + 2 + 3]

template <std::size_t byte_size, std::size_t current_size, typename Register,
          typename Op>
Register reduce_impl(Register x, Op op) {
  Register y = swap_adjacent<current_size>(x);
  x = op(x, y);
  if constexpr (current_size == byte_size) {
    return x;
  } else {
    return reduce_impl<byte_size, current_size / 2>(x, op);
  }
}

}  // namespace _reduce

template <typename T, std::size_t W, typename Op>
// require BinaryTransformation<Op, pack<T, W>>
pack<T, W> reduce(const pack<T, W>& x, Op op) {
  using pack_t = pack<T, W>;
  using reg = register_t<pack_t>;
  static constexpr std::size_t starting_size = sizeof(pack_t) / 2;

  reg res = _reduce::reduce_impl<sizeof(T), starting_size>(
      x.reg, [&](reg a, reg b) { return op(pack_t{a}, pack_t{b}).reg; });
  return pack_t{res};
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_REDUCE_H_
