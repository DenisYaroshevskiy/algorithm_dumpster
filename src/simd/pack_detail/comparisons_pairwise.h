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

#ifndef SIMD_PACK_DETAIL_COMPARISONS_PAIRWISE_H
#define SIMD_PACK_DETAIL_COMPARISONS_PAIRWISE_H

#include <type_traits>

#include "simd/bits.h"
#include "simd/pack_detail/pack_declaration.h"

namespace simd {

template <typename T, std::size_t W>
vbool_t<pack<T, W>> equal_pairwise(const pack<T, W>& x, const pack<T, W>& y) {
  return vbool_t<pack<T, W>>{mm::cmpeq<T>(x.reg, y.reg)};
}

template <typename T, std::size_t W>
vbool_t<pack<T, W>> greater_pairwise(const pack<T, W>& x, const pack<T, W>& y) {
  if constexpr (std::is_signed_v<T> || std::is_pointer_v<T>) {
    return vbool_t<pack<T, W>>{mm::cmpgt<T>(x.reg, y.reg)};
  } else {
    // https://stackoverflow.com/a/33173643/5021064

    using reg_t = register_t<pack<T, W>>;

    const reg_t convertion_mask = mm::set1<reg_t>(set_highest_4_bits<T>());

    const reg_t x_as_signed = mm::add<T>(x.reg, convertion_mask);
    const reg_t y_as_signed = mm::add<T>(y.reg, convertion_mask);

    using casted = std::conditional_t<std::is_same_v<T, uintptr_t>,
                                      std::int64_t, std::make_signed_t<T>>;

    return vbool_t<pack<T, W>>{mm::cmpgt<casted>(x_as_signed, y_as_signed)};
  }
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_COMPARISONS_PAIRWISE_H
