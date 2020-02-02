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

#ifndef SIMD_PACK_DETAIL_MINMAX_PAIRWISE_H
#define SIMD_PACK_DETAIL_MINMAX_PAIRWISE_H

#include "simd/pack_detail/comparisons_pairwise.h"
#include "simd/pack_detail/pack_declaration.h"

namespace simd {

template <typename T, std::size_t W>
pack<T, W> min_pairwise(const pack<T, W>& x, const pack<T, W>& y) {
 if constexpr (sizeof(T) < 8) {
    return pack<T, W>{mm::min<T>(x.reg, y.reg)};
  } else {
    // blend: if true take second.
    return blend(x, y, greater_pairwise(x, y));
  }
}

template <typename T, std::size_t W>
pack<T, W> max_pairwise(const pack<T, W>& x, const pack<T, W>& y) {
  if constexpr (sizeof(T) < 8) {
    return pack<T, W>{mm::max<T>(x.reg, y.reg)};
  } else {
    // blend: if true take second.
    return blend(y, x, greater_pairwise(x, y));
  }
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_MINMAX_PAIRWISE_H
