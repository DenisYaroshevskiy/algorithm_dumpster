/*
 * Copyright 2019 Oleg Fatkhiev, Denis Yaroshevskiy
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

#include "algo/binary_search.h"
#include "algo/type_functions.h"

namespace algo {

template <size_t... sizes>
struct uint_tuple;

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

template <typename Mask>
struct bit_info_t {
  Mask mask;
  size_t offset;
};

template <typename Result, size_t idx, size_t... sizes>
constexpr auto get_mask_and_offset() {
  constexpr auto arr = std::array{sizes...};
  auto offset =
      _uint_tuple::accumulate(arr.begin() + idx + 1, arr.end(), size_t{0});
  constexpr auto size = arr[idx];
  if constexpr (sizeof(Result) * CHAR_BIT <= size) {
    return bit_info_t<Result>{std::numeric_limits<Result>::max(), offset};
  } else {
    return bit_info_t<Result>{(Result{1} << size) - 1, offset};
  }
}

template <size_t idx, bool is_valid, size_t... sizes>
struct element_impl {};

template <size_t idx, size_t... sizes>
struct element_impl<idx, true, sizes...> {
  using type =
      uint_t<_uint_tuple::round_to_possible_size(std::array{sizes...}[idx])>;
};

template <size_t idx, typename Tuple>
struct element;

template <size_t idx, size_t... sizes>
struct element<idx, algo::uint_tuple<sizes...>>
    : algo::_uint_tuple::element_impl<idx, (idx < sizeof...(sizes)), sizes...> {
};

template <size_t idx, typename Tuple>
using element_t = typename element<idx, Tuple>::type;

template <typename Tuple, size_t... ids, typename... Args>
constexpr void construct(Tuple& t, std::index_sequence<ids...>, Args... args);

}  // namespace _uint_tuple

template <size_t... sizes>
struct uint_tuple {
 private:
  static_assert((... + sizes) <= supported_uint_sizes.back());

 public:
  using storage_type =
      uint_t<_uint_tuple::round_to_possible_size((... + sizes))>;

  constexpr uint_tuple() noexcept = default;

  template <typename... Ts,
            typename = std::enable_if_t<
                (std::is_convertible_v<
                     Ts, uint_t<_uint_tuple::round_to_possible_size(sizes)>> &&
                 ...)>>
  constexpr explicit uint_tuple(Ts... ts) : data(0) {
    _uint_tuple::construct(*this, std::index_sequence_for<Ts...>{}, ts...);
  }

  friend constexpr bool operator==(uint_tuple x, uint_tuple y) {
    return x.data == y.data;
  }
  friend constexpr bool operator!=(uint_tuple x, uint_tuple y) {
    return !(x == y);
  }
  friend constexpr bool operator<(uint_tuple x, uint_tuple y) {
    return x.data < y.data;
  }
  friend constexpr bool operator<=(uint_tuple x, uint_tuple y) {
    return !(y < x);
  }
  friend constexpr bool operator>(uint_tuple x, uint_tuple y) { return y < x; }
  friend constexpr bool operator>=(uint_tuple x, uint_tuple y) {
    return !(x < y);
  }

  storage_type data;
};

template <size_t idx, size_t... sizes>
constexpr auto get_at(uint_tuple<sizes...> t)
    -> _uint_tuple::element_t<idx, uint_tuple<sizes...>> {
  using storage_type = typename uint_tuple<sizes...>::storage_type;

  constexpr auto bit_info =
      _uint_tuple::get_mask_and_offset<storage_type, idx, sizes...>();
  return (t.data >> bit_info.offset) & bit_info.mask;
}

template <size_t idx, size_t... sizes>
constexpr void set_at(uint_tuple<sizes...>& t,
                      _uint_tuple::element_t<idx, uint_tuple<sizes...>> value) {
  using storage_type = typename uint_tuple<sizes...>::storage_type;

  constexpr auto bit_info =
      _uint_tuple::get_mask_and_offset<storage_type, idx, sizes...>();
  t.data &= ~(bit_info.mask << bit_info.offset);
  t.data |= (bit_info.mask & static_cast<storage_type>(value))
            << bit_info.offset;
}

namespace _uint_tuple {

template <typename Tuple, size_t... ids, typename... Args>
constexpr void construct(Tuple& t, std::index_sequence<ids...>, Args... args) {
  (..., set_at<ids>(t, args));
}

}  // namespace _uint_tuple

}  // namespace algo

namespace std {

template <size_t... sizes>
struct hash<algo::uint_tuple<sizes...>> {
  size_t operator()(algo::uint_tuple<sizes...> t) const noexcept {
    return std::hash<typename algo::uint_tuple<sizes...>::storage_type>{}(
        t.data);
  }
};

}  // namespace std

#endif  // ALGO_UINT_TUPLE_H
