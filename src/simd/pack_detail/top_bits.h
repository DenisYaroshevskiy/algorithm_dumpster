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

#ifndef SIMD_PACK_DETAIL_TOP_BITS_H_
#define SIMD_PACK_DETAIL_TOP_BITS_H_

#include <optional>

#include "simd/bits.h"
#include "simd/pack_detail/pack_declaration.h"

namespace simd {

template <typename Pack>
struct top_bits {
  std::uint32_t raw;

  explicit operator bool() const { return raw; }

  friend bool operator==(const top_bits& x, const top_bits& y) {
    return x.raw == y.raw;
  }

  friend bool operator!=(const top_bits& x, const top_bits& y) {
    return !(x == y);
  }

  friend bool operator<(const top_bits& x, const top_bits& y) {
    return x.raw < y.raw;
  }

  friend bool operator>(const top_bits& x, const top_bits& y) { return y < x; }

  friend bool operator<=(const top_bits& x, const top_bits& y) {
    return !(y < x);
  }

  friend bool operator>=(const top_bits& x, const top_bits& y) {
    return !(x < y);
  }

  friend top_bits& operator&=(top_bits& x, const top_bits& y) {
    x.raw &= y.raw;
    return x;
  }

  friend top_bits operator&(const top_bits& x, const top_bits& y) {
    top_bits tmp = x;
    tmp &= y;
    return tmp;
  }

  friend top_bits& operator|=(top_bits& x, const top_bits& y) {
    x.raw |= y.raw;
    return x;
  }

  friend top_bits operator|(const top_bits& x, const top_bits& y) {
    top_bits tmp = x;
    tmp |= y;
    return tmp;
  }

  friend top_bits operator~(const top_bits& x) { return top_bits{~x.raw}; }
};

template <typename Pack>
top_bits<Pack> get_top_bits(const Pack& x) {
  return top_bits<Pack>{
      static_cast<std::uint32_t>(mm::movemask<std::uint8_t>(x.reg))};
}

template <typename Pack>
top_bits<Pack> get_top_bits(const Pack& x, top_bits<Pack> ignore) {
  top_bits<Pack> res = get_top_bits(x);
  return res & ignore;
}

template <typename Pack>
top_bits<Pack> ignore_first_n_mask() {
  return top_bits<Pack>{set_lower_n_bits(sizeof(Pack))};
}

template <typename Pack>
top_bits<Pack> ignore_first_n_mask(std::uint32_t n) {
  return top_bits<Pack>{~set_lower_n_bits(n * sizeof(scalar_t<Pack>))};
}

template <typename Pack>
top_bits<Pack> ignore_first_n(const top_bits<Pack>& x) {
  return x;
}

template <typename Pack>
top_bits<Pack> ignore_first_n(const top_bits<Pack>& x, std::uint32_t n) {
  return x & ignore_first_n_mask<Pack>(n);
}

template <typename Pack>
top_bits<Pack> ignore_last_n_mask() {
  return ignore_first_n_mask<Pack>();
}

template <typename Pack>
top_bits<Pack> ignore_last_n_mask(std::uint32_t n) {
  return top_bits<Pack>{
      set_lower_n_bits(sizeof(Pack) - n * sizeof(scalar_t<Pack>))};
}

template <typename Pack>
top_bits<Pack> ignore_last_n(const top_bits<Pack>& x) {
  return x;
}

template <typename Pack>
top_bits<Pack> ignore_last_n(const top_bits<Pack>& x, std::uint32_t n) {
  return x & ignore_last_n_mask<Pack>(n);
}

template <typename Pack>
top_bits<Pack> combine_ignore(const top_bits<Pack>& x) {
  return x;
}

template <typename Pack>
top_bits<Pack> combine_ignore(const top_bits<Pack>& x, const top_bits<Pack>& ignore) {
  return x | ignore;
}

template <typename Pack>
std::optional<std::uint32_t> first_true(const top_bits<Pack>& x) {
  if (!x) return std::nullopt;
  return count_trailing_zeroes(x.raw) / sizeof(scalar_t<Pack>);
}

template <typename Pack>
bool all_true(const top_bits<Pack>& x) {
  return x.raw == set_lower_n_bits(sizeof(Pack));
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_TOP_BITS_H_
