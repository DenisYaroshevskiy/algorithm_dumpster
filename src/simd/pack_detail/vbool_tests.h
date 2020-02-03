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

#ifndef SIMD_PACK_DETAIL_VBOOL_TESTS_H_
#define SIMD_PACK_DETAIL_VBOOL_TESTS_H_

#include <optional>

#include "simd/bits.h"
#include "simd/pack_detail/pack_declaration.h"

namespace simd {
namespace _vbool_tests {

template <typename T, std::size_t W>
std::uint32_t movemask(const pack<T, W>& x) {
  return static_cast<std::uint32_t>(mm::movemask<std::uint8_t>(x.reg));
}

}  // _vbool_tests

template <typename T, std::size_t W>
bool all_true(const pack<T, W>& x) {
  return _vbool_tests::movemask(x) == set_lower_n_bits(sizeof(pack<T, W>));
}

template <typename T, std::size_t W>
bool any_true(const pack<T, W>& x) {
  return _vbool_tests::movemask(x);
}

template <typename T, std::size_t W>
bool any_true_ignore_first_n(const pack<T, W>& x, std::uint32_t n) {
  return _vbool_tests::movemask(x) & ~set_lower_n_bits(n * sizeof(T));
}


template <typename T, std::size_t W>
std::optional<std::uint32_t> first_true(const pack<T, W>& x) {
  auto mask = _vbool_tests::movemask(x);
  if (!mask) return std::nullopt;
  return count_trailing_zeroes(mask) / sizeof(T);
}

template <typename T, std::size_t W>
std::optional<std::uint32_t> first_true_ignore_first_n(const pack<T, W>& x, std::uint32_t n) {
  auto mask = _vbool_tests::movemask(x);
  mask &= ~set_lower_n_bits(n * sizeof(T));
  if (!mask) return std::nullopt;
  return count_trailing_zeroes(mask) / sizeof(T);
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_VBOOL_TESTS_H_
