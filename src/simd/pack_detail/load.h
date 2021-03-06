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

#ifndef SIMD_PACK_DETAIL_LOAD_H_
#define SIMD_PACK_DETAIL_LOAD_H_

#include <cstdint>
#include <utility>

#include "simd/pack_detail/pack_declaration.h"

namespace simd {

template <typename Pack, typename T>
Pack load(const T* addr) {
  using reg_t = register_t<Pack>;
  return Pack{mm::load(reinterpret_cast<const reg_t*>(addr))};
}

template <typename Pack, typename T>
Pack load_unaligned(const T* addr) {
  using reg_t = register_t<Pack>;
  return Pack{mm::loadu(reinterpret_cast<const reg_t*>(addr))};
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_LOAD_H_
