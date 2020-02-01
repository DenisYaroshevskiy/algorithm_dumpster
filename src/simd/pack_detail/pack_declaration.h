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

#ifndef SIMD_PACK_DETAIL_PACK_DECLARATION_H_
#define SIMD_PACK_DETAIL_PACK_DECLARATION_H_

#include <cstddef>
#include <type_traits>

#include "simd/mm.h"

namespace simd {
namespace _pack_declaration {

template <typename T>
auto select_vbool_scalar_type() {
  if constexpr (std::is_pointer_v<T>) {
    return std::uintptr_t{};
  } else {
    return std::make_unsigned_t<T>{};
  }
}

}  // namespace _pack_declaration

template <typename T, std::size_t W>
struct pack {
  // Type properties ==============
  using register_type = mm::register_i<W * sizeof(T) * 8>;

  using vbool_type =
      pack<decltype(_pack_declaration::select_vbool_scalar_type<T>()), W>;

  using value_type = T;

  static constexpr std::size_t size() { return W; }

  // Data  =========================

  register_type reg;
};

template <typename Pack>
using register_t = typename Pack::register_type;

template <typename Pack>
using vbool_t = typename Pack::vbool_type;

template <typename Pack>
using scalar_t = typename Pack::value_type;

template <typename Pack>
constexpr std::size_t size_v = Pack::size();

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_PACK_DECLARATION_H_
