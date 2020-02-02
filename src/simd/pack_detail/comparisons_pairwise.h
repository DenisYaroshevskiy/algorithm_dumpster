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

#ifndef SIMD_PACK_DETAIL_COMPARISONS_PAIRWISE_H_
#define SIMD_PACK_DETAIL_COMPARISONS_PAIRWISE_H_

#include <type_traits>

#include "simd/bits.h"
#include "simd/pack_detail/pack_declaration.h"
#include "simd/pack_detail/pack_cast.h"
#include "simd/pack_detail/set.h"

namespace simd {

template <typename T, std::size_t W>
vbool_t<pack<T, W>> equal_pairwise(const pack<T, W>& x, const pack<T, W>& y) {
  return vbool_t<pack<T, W>>{mm::cmpeq<T>(x.reg, y.reg)};
}

template <typename T, std::size_t W>
vbool_t<pack<T, W>> greater_pairwise(const pack<T, W>& x, const pack<T, W>& y) {
  if constexpr (asif_signed_v<T>) {
    return vbool_t<pack<T, W>>{mm::cmpgt<T>(x.reg, y.reg)};
  } else {
    // https://stackoverflow.com/a/33173643/5021064

    const auto convertion_mask = set_all<pack<T, W>>(set_highest_4_bits<T>());

    const auto _x = add_pairwise(x, convertion_mask);
    const auto _y = add_pairwise(y, convertion_mask);

    return greater_pairwise(cast_to_signed(_x), cast_to_signed(_y));
  }
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_COMPARISONS_PAIRWISE_H_
