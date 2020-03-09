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

#include "simd/mm.h"

#include <iostream>

namespace simd {
namespace _compress_mask {

// Some magic I stole from: https://stackoverflow.com/a/36951611/5021064
inline std::pair<std::uint64_t, std::uint8_t> mask8(std::uint8_t mask, std::uint64_t all_idxes) {
  const std::uint64_t mask_expanded = _pdep_u64(mask, 0x0101010101010101) * 0xff;
  const std::uint8_t offset = static_cast<std::uint8_t>(_mm_popcnt_u32(mask));
  const std::uint64_t compressed_idxs = _pext_u64(all_idxes, mask_expanded);
  return {compressed_idxs, offset};
}

inline std::pair<mm::register_i<128>, std::uint8_t> mask16(std::uint16_t mask) {
  auto [first_mask, first_offset] = mask8(mask & 0xff, 0x0706050403020100);
  auto [second_mask, second_offset] = mask8(mask >> 8, 0x0f0e0d0c0b0a0908);

  std::uint8_t bit_offset = first_offset * 8;
  std::uint64_t second_mask_mix = first_offset == 8 ? 0 : second_mask << bit_offset;

  first_mask = first_mask | second_mask_mix;
  second_mask >>= bit_offset;

  return { _mm_set_epi64x(second_mask, first_mask), first_offset + second_offset };
}

}  // namespace _compress_mask

template <typename Register>
std::pair<Register, std::uint8_t> compress_mask(std::uint32_t mmask) {
  static constexpr std::size_t register_width = mm::bit_width<Register>();
  if constexpr (register_width == 128) {
    return _compress_mask::mask16(mmask);
  } else {
    return {};
  }
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_COMPRESS_MASK_H_
