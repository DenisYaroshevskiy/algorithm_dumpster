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

#ifndef SIMD_PACK_DETAIL_REPLACE_IGNORED_H_
#define SIMD_PACK_DETAIL_REPLACE_IGNORED_H_

#include "simd/pack_detail/blend.h"
#include "simd/pack_detail/pack_declaration.h"
#include "simd/pack_detail/spread_top_bits.h"
#include "simd/pack_detail/top_bits.h"

namespace simd {

template <typename T, std::size_t W>
pack<T, W> replace_ignored(const pack<T, W>& x, const pack<T, W>&) {
  return x;
}

template <typename T, std::size_t W>
pack<T, W> replace_ignored(const pack<T, W>& x,
                           const top_bits<vbool_t<pack<T, W>>>& ignore_mask,
                           const pack<T, W>& with) {
  const auto vmask = spread_top_bits(ignore_mask);
  return blend(with, x, vmask);
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_REPLACE_IGNORED_H_
