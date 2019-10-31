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

#ifndef ALGO_TYPE_FUNCTIONS_H
#define ALGO_TYPE_FUNCTIONS_H

#include <array>
#include <climits>
#include <iterator>
#include <type_traits>

#include "compiler/introspection.h"

namespace algo {

template <typename T>
struct type_t {
  using type = T;
};

struct null_t {};

namespace _type_functions {

template <typename I>
struct is_move_iterator : std::false_type {};

template <typename I>
struct is_move_iterator<std::move_iterator<I>> : std::true_type {};

template <typename I>
struct is_reverse_iterator : std::false_type {};

template <typename I>
struct is_reverse_iterator<std::reverse_iterator<I>> : std::true_type {};

template <typename Op>
struct argument_type_impl {
  static constexpr auto call_pointer() { return &Op::operator(); }

  template <typename R, typename C, typename T, typename... Arg>
  static constexpr type_t<T> extract_arg(R (C::*)(T, Arg...)) {
    return {};
  }

  template <typename R, typename C, typename T, typename... Arg>
  static constexpr type_t<T> extract_arg(R (C::*)(T, Arg...) const) {
    return {};
  }

  using type = typename decltype(extract_arg(call_pointer()))::type;
};

template <size_t N>
constexpr auto uint_t_impl() {
  if constexpr (N == 8) {
    return type_t<std::uint8_t>{};
  } else if constexpr (N == 16) {
    return type_t<std::uint16_t>{};
  } else if constexpr (N == 32) {
    return type_t<std::uint32_t>{};
  } else if constexpr (N == 64) {
    return type_t<std::uint64_t>{};
  }
#ifdef HAS_128_INTS
  else if constexpr (N == 128) {
    return type_t<__uint128_t>{};
  }
#endif  // HAS_128_INTS
  else {
    return null_t{};
  }
}

}  // namespace _type_functions

template <typename I>
using ValueType = typename std::iterator_traits<I>::value_type;

template <typename I>
using DifferenceType = typename std::iterator_traits<I>::difference_type;

template <typename I>
using Pointer = typename std::iterator_traits<I>::pointer;

template <typename I>
using Reference = typename std::iterator_traits<I>::reference;

template <typename I>
using IteratorCategory = typename std::iterator_traits<I>::iterator_category;

template <typename I>
constexpr bool ForwardIterator =
    std::is_base_of_v<std::forward_iterator_tag, IteratorCategory<I>>;

template <typename I>
constexpr bool BidirectionalIterator =
    std::is_base_of_v<std::bidirectional_iterator_tag, IteratorCategory<I>>;

template <typename I>
constexpr bool RandomAccessIterator =
    std::is_base_of_v<std::random_access_iterator_tag, IteratorCategory<I>>;

template <typename I>
constexpr bool MoveIterator = _type_functions::is_move_iterator<I>::value;

template <typename I>
constexpr bool ReverseIterator = _type_functions::is_reverse_iterator<I>::value;

template <typename F>
// require UnaryCallable<F>
using ArgumentType = typename _type_functions::argument_type_impl<F>::type;

template <size_t N>
using uint_t = typename decltype(_type_functions::uint_t_impl<N>())::type;

inline constexpr std::array supported_uint_sizes = {
    size_t{8},   //
    size_t{16},  //
    size_t{32},  //
    size_t{64}   //
#ifdef HAS_128_INTS
    ,
    size_t{128}  //
#endif           // HAS_128_INTS
};

template <typename T>
constexpr auto uint_bit_size() -> std::enable_if_t<std::is_unsigned_v<T>, size_t> {
  auto* f = supported_uint_sizes.begin();
  while (f != supported_uint_sizes.end()) {
    if (*f == sizeof(T) * CHAR_BIT) return *f;
    ++f;
  }
  throw null_t{};  // compile time assert
}

}  // namespace algo

#endif  // ALGO_TYPE_FUNCTIONS_H
