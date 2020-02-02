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

#ifndef COMPILER_SIMD_H
#define COMPILER_SIMD_H

#include <array>
#include <cstdint>
#include <cstring>
#include <limits>
#include <type_traits>

#include "simd/mm.h"

namespace algo {
namespace _simd {

template <typename Vbool>
alignas(Vbool::alignment) static constexpr auto first_n_mask = [] {
  using vbool = Vbool;
  using bool_t = typename vbool::type;
  using array_t = typename vbool::array;

  constexpr bool_t FF =
      std::numeric_limits<std::make_unsigned_t<bool_t>>::max();

  std::array<array_t, vbool::width + 1> res = {};

  for (std::size_t i = 0; i != vbool::width + 1; ++i) {
    for (std::size_t j = 0; j != i; ++j) {
      res[i][j] = FF;
    }
  }

  return res;
}();

}  // namespace _simd

template <typename T, size_t W>
struct simd;

template <>
struct simd<std::int8_t, 16> {
  using reg_t = mm::register_i<128>;
  reg_t data;

  simd() = default;
  explicit simd(reg_t data) : data(data) {}

  using type = std::int8_t;
  using vbool = simd;
  using array = std::array<std::int8_t, 16>;

  static constexpr size_t width = mm::byte_width<reg_t>();
  static constexpr std::size_t alignment = mm::alignment<reg_t>();

  void load(const std::int8_t* addr) {
    data = mm::load_ignore_asan(reinterpret_cast<const reg_t*>(addr));
  }
  void store(std::int8_t* addr) {
    mm::store(reinterpret_cast<reg_t*>(addr), data); }
  void fill(std::int8_t x) { data = mm::set1<reg_t>(x); }
  void fill_0() { data = mm::setzero<reg_t>(); }

  friend simd pick_min(const simd& x, const simd& y) {
    return simd{mm::min<type>(x.data, y.data)};
  }

  friend simd pick_max(const simd& x, const simd& y) {
    return simd{mm::max<type>(x.data, y.data)};
  }

  friend vbool pairwise_equal(const simd& x, const simd& y) {
    return vbool{mm::cmpeq<type>(x.data, y.data)};
  }

  friend bool all_non_zero(const simd& x) {
    return mm::movemask<type>(x.data) == 0xffff;
  }

  friend bool all_zero(const simd& x) { return mm::movemask<type>(x.data) == 0; }

  friend int first_pairwise_equal(const simd& x, const simd& y) {
    return __builtin_ctz(mm::movemask<type>(pairwise_equal(x, y).data));
  }

  friend bool operator==(const simd& x, const simd& y) {
    return all_non_zero(pairwise_equal(x, y));
  }

  friend bool operator!=(const simd& x, const simd& y) { return !(x == y); }
};

template <typename Simd>
Simd blend_n_from_high(const Simd& x, const Simd& y, int n) {
  using vbool = typename Simd::vbool;

  vbool mask;
  mask.load(_simd::first_n_mask<vbool>[n].data());

  return Simd{mm::blendv<typename Simd::type>(x.data, y.data, mask.data)};
}

template <typename Simd>
Simd blend_n_from_low(const Simd& x, const Simd& y, int n) {
  return blend_n_from_high(y, x, Simd::width - n);
}

template <typename Simd>
Simd load_unaligned_with_filler(const typename Simd::type* addr,
                                typename Simd::type filler) {
  constexpr std::uintptr_t mask = ~(Simd::alignment - 1);

  const auto* aligned_addr = reinterpret_cast<decltype(addr)>(
      reinterpret_cast<std::uintptr_t>(addr) & mask);

  Simd res;
  res.load(aligned_addr);

  Simd filler_v;
  filler_v.fill(filler);

  return blend_n_from_high(res, filler_v, addr - aligned_addr);
}

template <typename Simd>
bool any_pairwise_equal(const Simd& x, const Simd& y) {
  return !all_zero(pairwise_equal(x, y));
}

}  // namespace algo

#endif  // COMPILER_SIMD_H
