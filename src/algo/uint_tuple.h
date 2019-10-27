/*
 * Copyright 2019 Denis Yaroshevskiy
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

#ifndef ALGO_UINT_TUPLE_H
#define ALGO_UINT_TUPLE_H

#include "algo/binary_search.h"
#include "algo/type_functions.h"

#include <array>
#include <functional>
#include <stdexcept>

namespace algo {
namespace _uint_tuple {

template <typename I, typename T, typename Op>
constexpr T accumulate(I f, I l, T x, Op op) {
  while (f != l) {
    x = op(std::move(x), *f);
    ++f;
  }
  return x;
}

template <typename I, typename T>
constexpr T accumulate(I f, I l, T x) {
  return _uint_tuple::accumulate(f, l, std::move(x), std::plus<>{});
}

constexpr size_t round_to_possible_size(size_t x) {
  return *algo::lower_bound(supported_uint_sizes.begin(),
                            supported_uint_sizes.end(), x);
}

constexpr bool is_power_of_2(size_t x) { return !(x & (x - 1)); }

}  // namespace _uint_tuple

template <size_t... sizes>
class uint_tuple {
  static_assert((... + sizes) <= supported_uint_sizes.back());
  static_assert((... && _uint_tuple::is_power_of_2(sizes)));

 public:
  using storage_type =
      uint_t<_uint_tuple::round_to_possible_size((... + sizes))>;

  storage_type data_;
};

template <size_t idx, size_t... sizes,
          typename Result = std::enable_if_t<(idx < sizeof...(sizes)),
                                             uint_t<std::array{sizes...}[idx]>>>
constexpr Result get_at(const uint_tuple<sizes...> t) {
  constexpr auto arr = std::array{sizes...};
  constexpr auto offset =
      _uint_tuple::accumulate(arr.begin(), arr.begin() + idx, size_t{0});
  return static_cast<Result>(t.data_ >> offset);
}

template <size_t idx, size_t... sizes,
          typename Income = std::enable_if_t<(idx < sizeof...(sizes)),
                                             uint_t<std::array{sizes...}[idx]>>>
constexpr void set_at(uint_tuple<sizes...>& t, Income value) {
  constexpr auto arr = std::array{sizes...};
  constexpr Income mask = -1;
  constexpr auto offset =
      _uint_tuple::accumulate(arr.begin(), arr.begin() + idx, size_t{0});
  t.data_ &= ~(static_cast<decltype(t.data_)>(mask) << offset);
  t.data_ |= static_cast<decltype(t.data_)>(value) << offset;
}

}  // namespace algo

#endif  // ALGO_UINT_TUPLE_H
