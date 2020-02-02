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


#ifndef SIMD_PACK_DETAIL_PACK_CAST_H_
#define SIMD_PACK_DETAIL_PACK_CAST_H_

#include <type_traits>

#include "simd/pack_detail/pack_declaration.h"

namespace simd {

template <typename Pack, typename T, std::size_t W>
Pack cast(const pack<T, W>& x) {
  static_assert(std::is_same_v<register_t<Pack>, register_t<pack<T, W>>>);
  return Pack{x.reg};
}

template <typename U, typename T, std::size_t W>
auto cast_elements(const pack<T, W>& x) {
  return cast<pack<U, W>>(x);
}

template <typename T, std::size_t W>
auto cast_to_bytes(const pack<T, W>& x) {
  return cast<pack<std::uint8_t, W * sizeof(T)>>(x);
}

template <typename T, std::size_t W>
auto cast_to_signed(const pack<T, W>& x) {
  return cast_elements<signed_equivalent<T>>(x);
}

template <typename T, std::size_t W>
auto cast_to_unsigned(const pack<T, W>& x) {
  return cast_elements<unsigned_equivalent<T>>(x);
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_PACK_CAST_H_
