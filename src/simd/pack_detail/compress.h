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

#ifndef SIMD_PACK_DETAIL_COMPRESS_H_
#define SIMD_PACK_DETAIL_COMPRESS_H_

#include <utility>

#include "simd/pack_detail/compress_mask.h"
#include "simd/pack_detail/pack_declaration.h"
#include "simd/pack_detail/set.h"
#include "simd/pack_detail/store.h"
#include "simd/pack_detail/top_bits.h"

namespace simd {
namespace _compress {

template <typename T, std::size_t W>
std::pair<pack<T, W / 2>, pack<T, W / 2>> split(const pack<T, W>& x) {
  return {{_mm256_extracti128_si256(x.reg, 0)},
          {_mm256_extracti128_si256(x.reg, 1)}};
}

template <typename T, typename Register>
inline Register blend_mask_from_shuffle(const Register& mask) {
  static constexpr std::size_t fits_count =
      mm::bit_width<Register>() / (8 * sizeof(T));
  using prepared_array = std::array<unsigned_equivalent<T>, fits_count>;

  // Not a load, get's optimized.
  alignas(Register) static constexpr prepared_array first_one_arr = {1};
  const auto first_one =
      mm::load(reinterpret_cast<const Register*>(first_one_arr.data()));

  const auto add_one_to_first = mm::add<std::int8_t>(mask, first_one);
  const auto zero = mm::setzero<Register>();

  return mm::cmpgt<std::int8_t>(add_one_to_first, zero);
}

}  // namespace _compress

template <typename T, std::size_t W>
T* compress_store_unsafe(T* out, const pack<T, W>& x,
                         top_bits<vbool_t<pack<T, W>>> mmask) {
  using reg_t = register_t<pack<T, W>>;

  if constexpr (mm::bit_width<reg_t>() == 256 && sizeof(T) >= 4) {
    auto [mask, offset] = compress_mask_for_permutevar8x32<T>(mmask.raw);
    const reg_t shuffled = _mm256_permutevar8x32_epi32(x.reg, mask);
    mm::storeu(reinterpret_cast<reg_t*>(out), shuffled);
    return out + offset;
  } else if constexpr(mm::bit_width<reg_t>() == 256) {
    auto [top, bottom] = _compress::split(x);
    using half_bits = top_bits<vbool_t<pack<T, W / 2>>>;

    out = compress_store_unsafe(out, top, half_bits{mmask.raw & 0xffff});
    return compress_store_unsafe(out, bottom, half_bits{mmask.raw >> 16});
  } else {
    auto [mask, offset] = compress_mask_for_shuffle_epi8<T>(mmask.raw);

    const reg_t shuffled = _mm_shuffle_epi8(x.reg, mask);
    mm::storeu(reinterpret_cast<reg_t*>(out), shuffled);

    return out + offset;
  }
}

// Copy pasting because of different checks for mmask != 0
// Also for compress_store_masked I don't utilize _mm256_permutevar8x32_epi32
// Should think more.

template <typename T, std::size_t W>
T* compress_store_masked(T* out, const pack<T, W>& x,
                         top_bits<vbool_t<pack<T, W>>> mmask) {
  using reg_t = register_t<pack<T, W>>;

  if constexpr (mm::bit_width<reg_t>() == 256) {
    auto [top, bottom] = _compress::split(x);
    using half_bits = top_bits<vbool_t<pack<T, W / 2>>>;

    out = compress_store_masked(out, top, half_bits{mmask.raw & 0xffff});
    return compress_store_masked(out, bottom, half_bits{mmask.raw >> 16});
  } else {
    // We have to do this check, since we can't in the end distinguish between
    // just taking the first element and not taking any elements.
    if (!mmask) return out;

    auto [mask, offset] = compress_mask_for_shuffle_epi8<T>(mmask.raw);

    const reg_t shuffled = _mm_shuffle_epi8(x.reg, mask);
    const reg_t store_mask = _compress::blend_mask_from_shuffle<T>(mask);

    mm::maskmoveu(reinterpret_cast<reg_t*>(out), shuffled, store_mask);
    return out + offset;
  }
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_COMPRESS_H_
