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

template <std::size_t W, typename T>
pack<T, W> load(const T* addr) {
  using reg_t = register_t<pack<T, W>>;
  return pack<T, W>{mm::load(reinterpret_cast<const reg_t*>(addr))};
}

template <std::size_t W, typename T>
pack<T, W> load_partial_miss(const T* addr) {
  using reg_t = register_t<pack<T, W>>;
  return pack<T, W>{mm::load_ignore_asan(reinterpret_cast<const reg_t*>(addr))};
}

template <std::size_t W, typename T>
std::pair<pack<T, W>, const T*> load_left_align(const T* addr) {
  constexpr std::uintptr_t mask = ~(alignof(pack<T, W>) - 1);

  addr =
      reinterpret_cast<const T*>(reinterpret_cast<std::uintptr_t>(addr) & mask);

  return std::pair{load_partial_miss<W>(addr), addr};
}

template <std::size_t W, typename T>
std::pair<pack<T, W>, T*> load_left_align(T* addr) {
  auto [pack, const_addr] = load_left_align<W>(static_cast<const T*>(addr));

  addr = const_cast<T*>(const_addr);
  return std::pair{pack, addr};
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_LOAD_H_
