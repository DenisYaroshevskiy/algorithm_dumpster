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

#ifndef SIMD_PACK_DETAIL_EQUAL_FULL_H_
#define SIMD_PACK_DETAIL_EQUAL_FULL_H_

#include "simd/pack_detail/masks.h"
#include "simd/pack_detail/pack_declaration.h"

namespace simd {

template <typename T, size_t W>
bool equal_full(const pack<T, W>& x, const pack<T, W>& y) {
  // For equality (for integers) we are OK to use bitwise equality.
  // We could also use memcmp, on clang that produced ~the same code.
  // However on gcc it didn't - I decided against it.

  using reg_t = register_t<pack<T, W>>;

  const reg_t byte_equal = mm::cmpeq<std::uint8_t>(x.reg, y.reg);
  const std::int32_t mmask = mm::movemask<std::uint8_t>(byte_equal);

  static constexpr std::int32_t ones = lower_n_bits_1(sizeof(pack<T, W>));
  return mmask == ones;
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_EQUAL_FULL_H_
