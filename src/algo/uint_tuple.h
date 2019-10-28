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

#include <array>
#include <climits>
#include <functional>
#include <stdexcept>

#include "algo/binary_search.h"
#include "algo/type_functions.h"

namespace algo {

template <size_t... sizes>
class uint_tuple;

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

template <size_t idx, size_t... sizes>
constexpr size_t get_offset() {
  auto arr = std::array{sizes...};
  return _uint_tuple::accumulate(arr.begin(), arr.begin() + idx, size_t{0});
}

template <typename Result, size_t size>
constexpr Result generate_mask() {
  if constexpr (sizeof(Result) * CHAR_BIT <= size) {
    return std::numeric_limits<Result>::max();
  } else {
    return (Result{1} << size) - 1;
  }
}

template <size_t idx, bool is_valid, size_t... sizes>
struct tuple_element_impl {};

template <size_t idx, size_t... sizes>
struct tuple_element_impl<idx, true, sizes...> {
  using type =
      uint_t<_uint_tuple::round_to_possible_size(std::array{sizes...}[idx])>;
};

template <size_t idx, typename Tuple>
struct tuple_element;

template <size_t idx, size_t... sizes>
struct tuple_element<idx, algo::uint_tuple<sizes...>>
    : algo::_uint_tuple::tuple_element_impl<idx, (idx < sizeof...(sizes)),
                                       sizes...> {};

template <size_t idx, typename Tuple>
using tuple_element_t = typename tuple_element<idx, Tuple>::type;

}  // namespace _uint_tuple

template <size_t... sizes>
class uint_tuple {
  static_assert((... + sizes) <= supported_uint_sizes.back());

 public:
  using storage_type =
      uint_t<_uint_tuple::round_to_possible_size((... + sizes))>;

  storage_type data_;
};

}  // namespace algo

namespace std {

template <size_t... sizes>
struct tuple_size<algo::uint_tuple<sizes...>>
    : std::integral_constant<size_t, sizeof...(sizes)> {};

}  // namespace std

namespace algo {

template <size_t idx, size_t... sizes>
constexpr auto get_at(uint_tuple<sizes...> t)
    -> _uint_tuple::tuple_element_t<idx, uint_tuple<sizes...>> {
  using storage_type = typename uint_tuple<sizes...>::storage_type;

  constexpr auto bit_size = std::array{sizes...}[idx];
  constexpr auto mask = _uint_tuple::generate_mask<storage_type, bit_size>();

  constexpr auto offset = _uint_tuple::get_offset<idx, sizes...>();
  return (t.data_ >> offset) & mask;
}

template <size_t idx, size_t... sizes>
constexpr void set_at(
    uint_tuple<sizes...>& t,
    _uint_tuple::tuple_element_t<idx, uint_tuple<sizes...>> value) {
  using storage_type = typename uint_tuple<sizes...>::storage_type;

  constexpr auto bit_size = std::array{sizes...}[idx];
  constexpr auto mask = _uint_tuple::generate_mask<storage_type, bit_size>();

  constexpr auto offset = _uint_tuple::get_offset<idx, sizes...>();
  t.data_ &= ~(mask << offset);
  t.data_ |= static_cast<storage_type>(value) << offset;
}

}  // namespace algo

#endif  // ALGO_UINT_TUPLE_H
