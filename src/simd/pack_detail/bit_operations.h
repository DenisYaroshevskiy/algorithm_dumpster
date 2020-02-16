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

#ifndef SIMD_PACK_DETAIL_BIT_OPERATIONS_H_
#define SIMD_PACK_DETAIL_BIT_OPERATIONS_H_

#include "simd/bits.h"
#include "simd/pack_detail/pack_declaration.h"
#include "simd/pack_detail/set.h"

namespace simd {

template <typename T, std::size_t W>
pack<T, W> and_(const pack<T, W>& x, const pack<T, W>& y) {
  return pack<T, W>{mm::and_(x.reg, y.reg)};
}

template <typename T, std::size_t W>
pack<T, W> or_(const pack<T, W>& x, const pack<T, W>& y) {
  return pack<T, W>{mm::or_(x.reg, y.reg)};
}

template <typename T, std::size_t W>
pack<T, W> xor_(const pack<T, W>& x, const pack<T, W>& y) {
  return pack<T, W>{mm::xor_(x.reg, y.reg)};
}

template <typename T, std::size_t W>
pack<T, W> not_x_and_y(const pack<T, W>& x, const pack<T, W>& y) {
  return pack<T, W>{mm::andnot(x.reg, y.reg)};
}

template <typename T, std::size_t W>
pack<T, W> not_(const pack<T, W>& x) {
  using uscalar = unsigned_equivalent<T>;
  const T FF = (T)all_ones<uscalar>();
  return not_x_and_y(x, set_all<pack<T, W>>(FF));
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_BIT_OPERATIONS_H_
