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

#ifndef SIMD_PACK_DETAIL_OPERATORS_H_
#define SIMD_PACK_DETAIL_OPERATORS_H_

#include <array>
#include <ostream>

#include "simd/pack_detail/comparisons.h"
#include "simd/pack_detail/pack_declaration.h"
#include "simd/pack_detail/store.h"

namespace simd {

template <typename T, std::size_t W>
bool operator==(const pack<T, W>& x, const pack<T, W>& y) {
  return equal_full(x, y);
}

template <typename T, std::size_t W>
bool operator!=(const pack<T, W>& x, const pack<T, W>& y) {
  return !(x == y);
}

template <typename T, std::size_t W>
bool operator<(const pack<T, W>& x, const pack<T, W>& y) {
  return less_full(x, y);
}

template <typename T, std::size_t W>
bool operator>(const pack<T, W>& x, const pack<T, W>& y) {
  return y < x;
}

template <typename T, std::size_t W>
bool operator<=(const pack<T, W>& x, const pack<T, W>& y) {
  return !(y < x);
}

template <typename T, std::size_t W>
bool operator>=(const pack<T, W>& x, const pack<T, W>& y) {
  return !(x < y);
}

template <typename T, std::size_t W>
std::ostream& operator<<(std::ostream& out, const pack<T, W>& x) {
  using scalar = scalar_t<pack<T, W>>;

  alignas(pack<T, W>) std::array<scalar, W> stored;
  store(stored.data(), x);

  out << '[' << stored[0];
  for (std::size_t i = 1; i != W; ++i) {
    out << ", " << stored[i];
  }
  out << ']';

  return out;
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_OPERATORS_H_
