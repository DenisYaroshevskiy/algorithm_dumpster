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

#ifndef SIMD_REGISTER_H
#define SIMD_REGISTER_H

#ifndef __AVX2__
#error "No AVX2 support"
#endif

#include <immintrin.h>

#include <cstdint>

#include "algo/type_functions.h"

namespace simd {
namespace _register {

template <std::size_t W>
constexpr auto register_i_impl() {
  if constexpr (W == 128) {
    return algo::type_t<__m128i>{};
  } else if constexpr (W == 256) {
    return algo::type_t<__m256i>{};
  } else {
    return algo::null_t{};
  }
}

}  // namespace _register

// type -----------------------------------------------------

template <std::size_t W>
using register_i = typename decltype(_register::register_i_impl<W>())::type;

// sizes -----------------------------------------------------------

template <typename Register>
constexpr size_t bit_width() {
  if constexpr (std::is_same_v<Register, register_i<128>>) {
    return 128;
  } else if constexpr (std::is_same_v<Register, register_i<256>>) {
    return 256;
  }
  throw algo::null_t{};  // compile time assert
}

template <typename Register>
constexpr size_t byte_width() {
  return bit_width<Register>() / 8;
}

template <typename Register>
constexpr size_t alignment() {
  return alignof(Register);
}

// load aligned ----------------------------------------------------

inline register_i<128> load_s(const register_i<128>* addr) {
  return _mm_load_si128(addr);
}

inline register_i<256> load_s(const register_i<256>* addr) {
  return _mm256_load_si256(addr);
}

// store aligned ----------------------------------------------------

inline void store_s(register_i<128>* addr, register_i<128> x) {
  _mm_store_si128(addr, x);
}

inline void store_s(register_i<256>* addr, register_i<256> x) {
  _mm256_store_si256(addr, x);
}

// set --------------------------------------------------------------

template <size_t register_width, typename T>
inline auto set1(T a) {
  static constexpr size_t t_width = algo::bit_size<T>();

  if constexpr (register_width == 128) {
    // clang-format off
    if constexpr      (t_width == 8)  return _mm_set1_epi8(a);
    else if constexpr (t_width == 16) return _mm_set1_epi16(a);
    else if constexpr (t_width == 32) return _mm_set1_epi32(a);
    else if constexpr (t_width == 64) return _mm_set1_epi64x(a);
    else                              return algo::null_t{};
    // clang-format on
  } else if constexpr (register_width == 256) {
    // clang-format off
    if constexpr      (t_width == 8)  return _mm256_set1_epi8(a);
    else if constexpr (t_width == 16) return _mm256_set1_epi16(a);
    else if constexpr (t_width == 32) return _mm256_set1_epi32(a);
    else if constexpr (t_width == 64) return _mm256_set1_epi64x(a);
    else                              return algo::null_t{};
    // clang-format on
  } else {
    return algo::null_t{};
  }
}

}  // namespace simd

#endif  // SIMD_REGISTER_H
