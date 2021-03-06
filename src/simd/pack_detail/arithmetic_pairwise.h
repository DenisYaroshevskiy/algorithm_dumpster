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

#ifndef SIMD_PACK_DETAIL_ARITHMETIC_PAIRWISE_H_
#define SIMD_PACK_DETAIL_ARITHMETIC_PAIRWISE_H_

#include "simd/pack_detail/pack_declaration.h"

namespace simd {

template <typename T, std::size_t W>
pack<T, W> add_pairwise(const pack<T, W>& x, const pack<T, W>& y) {
  return pack<T, W>{mm::add<T>(x.reg, y.reg)};
}

template <typename T, std::size_t W>
pack<T, W> sub_pairwise(const pack<T, W>& x, const pack<T, W>& y) {
  return pack<T, W>{mm::sub<T>(x.reg, y.reg)};
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_ARITHMETIC_PAIRWISE_H_
