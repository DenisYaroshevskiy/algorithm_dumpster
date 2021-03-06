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

#include "simd/pack_detail/arithmetic_pairwise.h"
#include "simd/pack_detail/bit_operations.h"
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
  return less_lexicographical(x, y);
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
pack<T, W>& operator+=(pack<T, W>& x, const pack<T, W>& y) {
  x = add_pairwise(x, y);
  return x;
}

template <typename T, std::size_t W>
pack<T, W> operator+(const pack<T, W>& x, const pack<T, W>& y) {
  return add_pairwise(x, y);
}

template <typename T, std::size_t W>
pack<T, W>& operator-=(pack<T, W>& x, const pack<T, W>& y) {
  x = sub_pairwise(x, y);
  return x;
}

template <typename T, std::size_t W>
pack<T, W> operator-(const pack<T, W>& x, const pack<T, W>& y) {
  return sub_pairwise(x, y);
}

template <typename T, std::size_t W>
pack<T, W>& operator&=(pack<T, W>& x, const pack<T, W>& y) {
  x = and_(x, y);
  return x;
}

template <typename T, std::size_t W>
pack<T, W> operator&(const pack<T, W>& x, const pack<T, W>& y) {
  return and_(x, y);
}

template <typename T, std::size_t W>
pack<T, W>& operator|=(pack<T, W>& x, const pack<T, W>& y) {
  x = or_(x, y);
  return x;
}

template <typename T, std::size_t W>
pack<T, W> operator|(const pack<T, W>& x, const pack<T, W>& y) {
  return or_(x, y);
}

template <typename T, std::size_t W>
pack<T, W>& operator^=(pack<T, W>& x, const pack<T, W>& y) {
  x = xor_(x, y);
  return x;
}

template <typename T, std::size_t W>
pack<T, W> operator^(const pack<T, W>& x, const pack<T, W>& y) {
  return xor_(x, y);
}

template <typename T, std::size_t W>
pack<T, W> operator~(const pack<T, W>& x) {
  return not_(x);
}

template <typename T, std::size_t W>
std::ostream& operator<<(std::ostream& out, const pack<T, W>& x) {
  using scalar = scalar_t<pack<T, W>>;

  alignas(pack<T, W>) std::array<scalar, W> stored;
  store(stored.data(), x);

  auto print_t = [&](T elem) {
    if constexpr(std::is_same_v<T, char> || std::is_same_v<T, unsigned char>) {
      out << '(' << int(elem) << ')';
    } else {
      out << elem;
    }
  };

  out << '[';
  print_t(stored[0]);
  for (std::size_t i = 1; i != W; ++i) {
    out << ", ";
    print_t(stored[i]);
  }
  out << ']';

  return out;
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_OPERATORS_H_
