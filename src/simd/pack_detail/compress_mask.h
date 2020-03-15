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

#include <algorithm>
#include <tuple>

namespace simd {
namespace _compress_mask {

// Some magic I stole from: https://stackoverflow.com/a/36951611/5021064
inline std::pair<std::uint64_t, std::uint8_t> mask8(std::uint8_t mask,
                                                    std::uint64_t all_idxes) {
  const std::uint64_t mask_expanded =
      _pdep_u64(mask, 0x0101010101010101) * 0xff;

  const std::uint8_t offset = static_cast<std::uint8_t>(_mm_popcnt_u32(mask));
  const std::uint64_t compressed_idxs = _pext_u64(all_idxes, mask_expanded);

  return {compressed_idxs, offset};
}

constexpr void shift_two_masks8(std::uint64_t& first_mask,
                                std::uint64_t& second_mask,
                                std::uint8_t first_n) {
  first_mask |= first_n == 8 ? 0 : second_mask << (first_n * 8);
  second_mask = first_n == 0 ? 0 : second_mask >> ((8 - first_n) * 8) ;
}

constexpr void shift_offsets8(std::uint8_t& first_n, std::uint8_t& second_n) {
  const std::uint8_t shift =
      std::min(static_cast<std::uint8_t>(8 - first_n), second_n);
  first_n += shift;
  second_n -= shift;
}

constexpr std::uint64_t make_8_idexes(std::uint8_t offset) {
  const std::uint64_t basic = 0x0706050403020100;
  const std::uint64_t plus1 = 0x0101010101010101;
  return basic + plus1 * offset;
}

inline std::pair<mm::register_i<128>, std::uint8_t> mask16(std::uint16_t mask) {
  auto [first_mask, first_n] = mask8(mask & 0xff, make_8_idexes(0));
  auto [second_mask, second_n] = mask8(mask >> 8, make_8_idexes(8));

  shift_two_masks8(first_mask, second_mask, first_n);

  return {_mm_set_epi64x(second_mask, first_mask), first_n + second_n};
}

inline std::pair<mm::register_i<256>, std::uint8_t> mask32(std::uint32_t mask) {
  alignas(mm::register_i<256>) std::array<std::uint64_t, 4> masks = {};
  std::array<std::uint8_t, 4> ns;

  std::tie(masks[0], ns[0]) = mask8(mask & 0xff, make_8_idexes(0));
  std::uint8_t last = 1;
  std::uint8_t total = ns[0];

  auto combine = [&](std::uint8_t mask_part, std::uint8_t idx_offset) mutable {
    std::tie(masks[last], ns[last]) =
        mask8(mask_part, make_8_idexes(idx_offset));
    shift_two_masks8(masks[last - 1], masks[last], ns[last - 1]);

    total += ns[last];
    shift_offsets8(ns[last - 1], ns[last]);
    last += (ns[last] != 0);
  };

  combine((mask >> 8) & 0xff, 8);
  combine((mask >> 16) & 0xff, 16);
  combine(mask >> 24, 24);

  return {_mm256_set_epi64x(masks[3], masks[2], masks[1], masks[0]), total};
}

}  // namespace _compress_mask

template <typename Register>
std::pair<Register, std::uint8_t> compress_mask(std::uint32_t mmask) {
  static constexpr std::size_t register_width = mm::bit_width<Register>();
  if constexpr (register_width == 128) {
    return _compress_mask::mask16(static_cast<std::uint16_t>(mmask));
  } else {
    return _compress_mask::mask32(mmask);
  }
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_COMPRESS_MASK_H_
