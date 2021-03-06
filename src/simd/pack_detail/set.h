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

#ifndef SIMD_PACK_DETAIL_SET_H_
#define SIMD_PACK_DETAIL_SET_H_

#include "simd/pack_detail/pack_declaration.h"

namespace simd {

template <typename Pack>
Pack set_all(scalar_t<Pack> x) {
  return Pack{mm::set1<register_t<Pack>>(x)};
}

template <typename Pack>
Pack set_zero() {
  return Pack{mm::setzero<register_t<Pack>>()};
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_SET_H_
