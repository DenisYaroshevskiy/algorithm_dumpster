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

/*
 * ================================================================
 *
 *  GENERATED CODE. Script: ../src/simd/mm_operations_generator.py
 *
 * ================================================================
 */
#ifndef SIMD_MM_H_
#define SIMD_MM_H_

#include <immintrin.h>

#include <cstddef>
#include <cstdint>
#include <type_traits>

namespace mm {

struct error_t {};

template <typename T>
struct type_t {
  using type = T;
};

// Helper to support pointers.
template <typename T, typename Int>
constexpr bool is_equivalent() {
  if (sizeof(T) != sizeof(Int)) return false;
  if (std::is_signed_v<Int>) return std::is_signed_v<T> || std::is_pointer_v<T>;
  return !std::is_signed_v<T>;
}

// register_i ------------------------------

namespace _mm {

template <std::size_t W>
constexpr auto register_i_impl() {
  if constexpr (W == 128)
    return type_t<__m128i>{};
  else if constexpr (W == 256)
    return type_t<__m256i>{};
  else if constexpr (W == 512)
    return type_t<__m512i>{};
  else
    return error_t{};
}
}  // namespace _mm

template <std::size_t W>
using register_i = typename decltype(_mm::register_i_impl<W>())::type;

// sizes -----------------------------------

template <typename Register>
constexpr std::size_t bit_width() {
  if constexpr (std::is_same_v<Register, register_i<128>>) {
    return 128;
  } else if constexpr (std::is_same_v<Register, register_i<256>>) {
    return 256;
  } else if constexpr (std::is_same_v<Register, register_i<512>>) {
    return 512;
  }
  throw error_t{};
}

template <typename Register>
constexpr size_t byte_width() {
  return bit_width<Register>() / 8;
}

template <typename Register>
constexpr size_t alignment() {
  return alignof(Register);
}

// load/store ------------------------------

inline register_i<128> load(const register_i<128>* addr) {
  return _mm_load_si128(addr);
}

inline register_i<256> load(const register_i<256>* addr) {
  return _mm256_load_si256(addr);
}

inline register_i<512> load(const register_i<512>* addr) {
  return _mm512_load_si512(addr);
}

__attribute__((no_sanitize_address)) inline register_i<128> load_ignore_asan(
    const register_i<128>* addr) {
  return _mm_load_si128(addr);
}

__attribute__((no_sanitize_address)) inline register_i<256> load_ignore_asan(
    const register_i<256>* addr) {
  return _mm256_load_si256(addr);
}

__attribute__((no_sanitize_address)) inline register_i<512> load_ignore_asan(
    const register_i<512>* addr) {
  return _mm512_load_si512(addr);
}

inline void store(register_i<128>* addr, register_i<128> a) {
  _mm_store_si128(addr, a);
}

inline void store(register_i<256>* addr, register_i<256> a) {
  _mm256_store_si256(addr, a);
}

inline void store(register_i<512>* addr, register_i<512> a) {
  _mm512_store_si512(addr, a);
}

// set one value everywhere ----------------

// Does not exist for floats.
template <typename Register>
inline auto setzero() {
  static constexpr size_t register_width = bit_width<Register>();
  if constexpr (register_width == 128)
    return _mm_setzero_si128();
  else if constexpr (register_width == 256)
    return _mm256_setzero_si256();
  else if constexpr (register_width == 512)
    return _mm512_setzero_si512();
  else
    return error_t{};
}

template <typename Register, typename T>
inline auto set1(T a) {
  static constexpr std::size_t register_width = bit_width<Register>();
  static constexpr std::size_t t_width = sizeof(T) * 8;

  if constexpr (register_width == 128 && t_width == 8)
    return _mm_set1_epi8((std::int8_t)a);
  else if constexpr (register_width == 128 && t_width == 16)
    return _mm_set1_epi16((std::int16_t)a);
  else if constexpr (register_width == 128 && t_width == 32)
    return _mm_set1_epi32((std::int32_t)a);
  else if constexpr (register_width == 128 && t_width == 64)
    return _mm_set1_epi64x((std::int64_t)a);
  else if constexpr (register_width == 256 && t_width == 8)
    return _mm256_set1_epi8((std::int8_t)a);
  else if constexpr (register_width == 256 && t_width == 16)
    return _mm256_set1_epi16((std::int16_t)a);
  else if constexpr (register_width == 256 && t_width == 32)
    return _mm256_set1_epi32((std::int32_t)a);
  else if constexpr (register_width == 256 && t_width == 64)
    return _mm256_set1_epi64x((std::int64_t)a);
  else if constexpr (register_width == 512 && t_width == 8)
    return _mm512_set1_epi8((std::int8_t)a);
  else if constexpr (register_width == 512 && t_width == 16)
    return _mm512_set1_epi16((std::int16_t)a);
  else if constexpr (register_width == 512 && t_width == 32)
    return _mm512_set1_epi32((std::int32_t)a);
  else if constexpr (register_width == 512 && t_width == 64)
    return _mm512_set1_epi64((std::int64_t)a);
  else
    return error_t{};
}

// min/max ---------------------------------

template <typename T, typename Register>
inline auto min(Register a, Register b) {
  static constexpr size_t register_width = bit_width<Register>();

  if constexpr (register_width == 128 && is_equivalent<T, std::int8_t>())
    return _mm_min_epi8(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::uint8_t>())
    return _mm_min_epu8(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::int16_t>())
    return _mm_min_epi16(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::uint16_t>())
    return _mm_min_epu16(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::int32_t>())
    return _mm_min_epi32(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::uint32_t>())
    return _mm_min_epu32(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::int64_t>())
    return _mm_min_epi64(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::uint64_t>())
    return _mm_min_epu64(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::int8_t>())
    return _mm256_min_epi8(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::uint8_t>())
    return _mm256_min_epu8(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::int16_t>())
    return _mm256_min_epi16(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::uint16_t>())
    return _mm256_min_epu16(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::int32_t>())
    return _mm256_min_epi32(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::uint32_t>())
    return _mm256_min_epu32(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::int64_t>())
    return _mm256_min_epi64(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::uint64_t>())
    return _mm256_min_epu64(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::int8_t>())
    return _mm512_min_epi8(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::uint8_t>())
    return _mm512_min_epu8(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::int16_t>())
    return _mm512_min_epi16(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::uint16_t>())
    return _mm512_min_epu16(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::int32_t>())
    return _mm512_min_epi32(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::uint32_t>())
    return _mm512_min_epu32(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::int64_t>())
    return _mm512_min_epi64(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::uint64_t>())
    return _mm512_min_epu64(a, b);
  else
    return error_t{};
}

template <typename T, typename Register>
inline auto max(Register a, Register b) {
  static constexpr size_t register_width = bit_width<Register>();

  if constexpr (register_width == 128 && is_equivalent<T, std::int8_t>())
    return _mm_max_epi8(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::uint8_t>())
    return _mm_max_epu8(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::int16_t>())
    return _mm_max_epi16(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::uint16_t>())
    return _mm_max_epu16(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::int32_t>())
    return _mm_max_epi32(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::uint32_t>())
    return _mm_max_epu32(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::int64_t>())
    return _mm_max_epi64(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::uint64_t>())
    return _mm_max_epu64(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::int8_t>())
    return _mm256_max_epi8(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::uint8_t>())
    return _mm256_max_epu8(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::int16_t>())
    return _mm256_max_epi16(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::uint16_t>())
    return _mm256_max_epu16(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::int32_t>())
    return _mm256_max_epi32(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::uint32_t>())
    return _mm256_max_epu32(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::int64_t>())
    return _mm256_max_epi64(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::uint64_t>())
    return _mm256_max_epu64(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::int8_t>())
    return _mm512_max_epi8(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::uint8_t>())
    return _mm512_max_epu8(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::int16_t>())
    return _mm512_max_epi16(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::uint16_t>())
    return _mm512_max_epu16(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::int32_t>())
    return _mm512_max_epi32(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::uint32_t>())
    return _mm512_max_epu32(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::int64_t>())
    return _mm512_max_epi64(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::uint64_t>())
    return _mm512_max_epu64(a, b);
  else
    return error_t{};
}

// comparisons -----------------------------

template <typename T, typename Register>
inline auto cmpeq(Register a, Register b) {
  static constexpr size_t register_width = bit_width<Register>();
  static constexpr size_t t_width = sizeof(T) * 8;
  if constexpr (register_width == 128 && t_width == 8)
    return _mm_cmpeq_epi8(a, b);
  else if constexpr (register_width == 128 && t_width == 16)
    return _mm_cmpeq_epi16(a, b);
  else if constexpr (register_width == 128 && t_width == 32)
    return _mm_cmpeq_epi32(a, b);
  else if constexpr (register_width == 128 && t_width == 64)
    return _mm_cmpeq_epi64(a, b);
  else if constexpr (register_width == 256 && t_width == 8)
    return _mm256_cmpeq_epi8(a, b);
  else if constexpr (register_width == 256 && t_width == 16)
    return _mm256_cmpeq_epi16(a, b);
  else if constexpr (register_width == 256 && t_width == 32)
    return _mm256_cmpeq_epi32(a, b);
  else if constexpr (register_width == 256 && t_width == 64)
    return _mm256_cmpeq_epi64(a, b);
  else if constexpr (register_width == 512 && t_width == 8)
    return _mm512_cmpeq_epi8(a, b);
  else if constexpr (register_width == 512 && t_width == 16)
    return _mm512_cmpeq_epi16(a, b);
  else if constexpr (register_width == 512 && t_width == 32)
    return _mm512_cmpeq_epi32(a, b);
  else if constexpr (register_width == 512 && t_width == 64)
    return _mm512_cmpeq_epi64(a, b);
  else
    return error_t{};
}

// Instruction is not avaliable for unsigned ints.
template <typename T, typename Register>
inline auto cmpgt(Register a, Register b) {
  static constexpr size_t register_width = bit_width<Register>();
  if constexpr (register_width == 128 && is_equivalent<T, std::int8_t>())
    return _mm_cmpgt_epi8(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::int16_t>())
    return _mm_cmpgt_epi16(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::int32_t>())
    return _mm_cmpgt_epi32(a, b);
  else if constexpr (register_width == 128 && is_equivalent<T, std::int64_t>())
    return _mm_cmpgt_epi64(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::int8_t>())
    return _mm256_cmpgt_epi8(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::int16_t>())
    return _mm256_cmpgt_epi16(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::int32_t>())
    return _mm256_cmpgt_epi32(a, b);
  else if constexpr (register_width == 256 && is_equivalent<T, std::int64_t>())
    return _mm256_cmpgt_epi64(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::int8_t>())
    return _mm512_cmpgt_epi8(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::int16_t>())
    return _mm512_cmpgt_epi16(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::int32_t>())
    return _mm512_cmpgt_epi32(a, b);
  else if constexpr (register_width == 512 && is_equivalent<T, std::int64_t>())
    return _mm512_cmpgt_epi64(a, b);
  else
    return error_t{};
}

// add/sub ---------------------------------

template <typename T, typename Register>
inline auto add(Register a, Register b) {
  static constexpr size_t register_width = bit_width<Register>();
  static constexpr size_t t_width = sizeof(T) * 8;
  if constexpr (register_width == 128 && t_width == 8)
    return _mm_add_epi8(a, b);
  else if constexpr (register_width == 128 && t_width == 16)
    return _mm_add_epi16(a, b);
  else if constexpr (register_width == 128 && t_width == 32)
    return _mm_add_epi32(a, b);
  else if constexpr (register_width == 128 && t_width == 64)
    return _mm_add_epi64(a, b);
  else if constexpr (register_width == 256 && t_width == 8)
    return _mm256_add_epi8(a, b);
  else if constexpr (register_width == 256 && t_width == 16)
    return _mm256_add_epi16(a, b);
  else if constexpr (register_width == 256 && t_width == 32)
    return _mm256_add_epi32(a, b);
  else if constexpr (register_width == 256 && t_width == 64)
    return _mm256_add_epi64(a, b);
  else if constexpr (register_width == 512 && t_width == 8)
    return _mm512_add_epi8(a, b);
  else if constexpr (register_width == 512 && t_width == 16)
    return _mm512_add_epi16(a, b);
  else if constexpr (register_width == 512 && t_width == 32)
    return _mm512_add_epi32(a, b);
  else if constexpr (register_width == 512 && t_width == 64)
    return _mm512_add_epi64(a, b);
  else
    return error_t{};
}

template <typename T, typename Register>
inline auto sub(Register a, Register b) {
  static constexpr size_t register_width = bit_width<Register>();
  static constexpr size_t t_width = sizeof(T) * 8;
  if constexpr (register_width == 128 && t_width == 8)
    return _mm_sub_epi8(a, b);
  else if constexpr (register_width == 128 && t_width == 16)
    return _mm_sub_epi16(a, b);
  else if constexpr (register_width == 128 && t_width == 32)
    return _mm_sub_epi32(a, b);
  else if constexpr (register_width == 128 && t_width == 64)
    return _mm_sub_epi64(a, b);
  else if constexpr (register_width == 256 && t_width == 8)
    return _mm256_sub_epi8(a, b);
  else if constexpr (register_width == 256 && t_width == 16)
    return _mm256_sub_epi16(a, b);
  else if constexpr (register_width == 256 && t_width == 32)
    return _mm256_sub_epi32(a, b);
  else if constexpr (register_width == 256 && t_width == 64)
    return _mm256_sub_epi64(a, b);
  else if constexpr (register_width == 512 && t_width == 8)
    return _mm512_sub_epi8(a, b);
  else if constexpr (register_width == 512 && t_width == 16)
    return _mm512_sub_epi16(a, b);
  else if constexpr (register_width == 512 && t_width == 32)
    return _mm512_sub_epi32(a, b);
  else if constexpr (register_width == 512 && t_width == 64)
    return _mm512_sub_epi64(a, b);
  else
    return error_t{};
}

// movemask --------------------------------

template <typename T, typename Register>
inline auto movemask(Register a) {
  static constexpr size_t register_width = bit_width<Register>();
  static constexpr size_t t_width = sizeof(T) * 8;

  if constexpr (register_width == 128 && t_width == 8)
    return _mm_movemask_epi8(a);
  else if constexpr (register_width == 256 && t_width == 8)
    return _mm256_movemask_epi8(a);
  else
    return error_t{};
}

template <typename T, typename Register>
inline auto blendv(Register a, Register b, Register mask) {
  static constexpr size_t register_width = bit_width<Register>();
  static constexpr size_t t_width = sizeof(T) * 8;

  if constexpr (register_width == 128 && t_width == 8)
    return _mm_blendv_epi8(a, b, mask);
  else if constexpr (register_width == 256 && t_width == 8)
    return _mm256_blendv_epi8(a, b, mask);
  else
    return error_t{};
}

}  // namespace mm

#endif  // SIMD_MM_H_
