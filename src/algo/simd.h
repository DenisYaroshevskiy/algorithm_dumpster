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

#ifndef COMPILER_SIMD_H
#define COMPILER_SIMD_H

#ifndef __AVX2__
#error "No AVX2 support"
#endif

#include <immintrin.h>
#include <cstdint>
#include <cstring>

namespace algo {

template <typename T, size_t W>
struct simd;

template <>
struct simd<std::int8_t, 16> {
  __m128i data;

  using vbool = simd;

  simd() = default;
  explicit simd(__m128i data) : data(data) {}

  void load(const std::int8_t* addr) { std::memcpy(&data, addr, sizeof(data)); }

  void fill(std::int8_t x) {
    data = _mm_set1_epi8(x);
  }

  friend simd pick_min(const simd& x, const simd& y) {
    return simd{_mm_min_epi8(x.data, y.data)};
  }

  friend simd pick_max(const simd& x, const simd& y) {
    return simd{_mm_max_epi8(x.data, y.data)};
  }

  friend vbool pairwise_equal(const simd& x, const simd& y) {
    return vbool{_mm_cmpeq_epi8(x.data, y.data)};
  }

  friend bool all_non_zero(const simd& x) {
    return _mm_movemask_epi8(x.data) == 0xffff;
  }

  friend bool operator==(const simd& x, const simd& y) {
    return all_non_zero(pairwise_equal(x, y));
  }

  friend bool operator!=(const simd& x, const simd& y) { return !(x == y); }
};

}  // namespace algo

#endif  // COMPILER_SIMD_H
