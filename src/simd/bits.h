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

#ifndef SIMD_PACK_DETAIL_BITS_H_
#define SIMD_PACK_DETAIL_BITS_H_

#include <cstdint>
#include <type_traits>

namespace simd {

inline std::int32_t count_trailing_zeroes(std::uint32_t x) {
  return __builtin_ctz(x);
}

// https://stackoverflow.com/questions/18806481/how-can-i-get-the-position-of-the-least-significant-bit-in-a-number
inline std::uint32_t lsb(std::uint32_t x) {
  return x & -x;
}

// Like a regular < but a less significant bit is treated as
// more significant.
inline bool lsb_less(std::uint32_t x, std::uint32_t y) {
  // zero out common bits.
  const std::uint32_t unequal_bits = x ^ y;
  x &= unequal_bits;
  y &= unequal_bits;

  if (y == 0) return false;
  if (x == 0) return true;

  return lsb(x) > lsb(y);
}

constexpr std::uint32_t set_lower_n_bits(std::uint32_t n) {
  std::uint64_t res{1};
  res <<= n;
  res -= 1;
  return static_cast<std::uint32_t>(res);
}

template <typename N>
constexpr N set_highest_4_bits() {
  N res = 0x80;
  res <<= (sizeof(N) - 1) * 8;
  return res;
}

template <typename N>
constexpr N all_ones() {
  return ~N{0};
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_BITS_H_
