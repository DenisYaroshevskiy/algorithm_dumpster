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

#ifndef SIMD_PAIRWISE_EQUAL_H
#define SIMD_PAIRWISE_EQUAL_H

#include "simd/pack_detail/pack_declaration.h"

namespace simd {

template <typename T, std::size_t W>
inline auto equal_pairwise(const pack<T, W>& x, const pack<T, W>& y) {
  using res_t = vbool_t<pack<T, W>>;
  return res_t{mm::cmpeq<T>(x.reg, y.reg)};
}

}  // namespace simd

#endif  // SIMD_PAIRWISE_EQUAL_H
