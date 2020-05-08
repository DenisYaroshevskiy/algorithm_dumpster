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

#ifndef UNSQ_DRILL_DOWN_H_
#define UNSQ_DRILL_DOWN_H_

#include <cstring>
#include <iterator>
#include <type_traits>
#include <utility>

namespace unsq {
namespace _drill_down {

struct error_t {};

template <typename T>
struct type_t {
  using type = T;
};

template <typename T>
constexpr auto equivalent() {
  if constexpr (std::is_integral_v<T>)
    return type_t<T>{};
  else if constexpr (std::is_enum_v<T>)
    return type_t<std::underlying_type<T>>{};
  else if constexpr (std::is_pointer_v<T>) {
    static_assert(sizeof(T) == sizeof(std::int64_t));
    return type_t<std::int64_t>{};
  }
  // you asked for this
  else if constexpr (sizeof(T) == 1)
    return type_t<std::uint8_t>{};
  else if constexpr (sizeof(T) == 2)
    return type_t<std::uint16_t>{};
  else if constexpr (sizeof(T) == 4)
    return type_t<std::uint32_t>{};
  else if constexpr (sizeof(T) == 8)
    return type_t<std::uint64_t>{};
  else
    return error_t{};
}

template <typename>
struct is_const_pointer : std::false_type {};

template <typename T>
struct is_const_pointer<const T*> : std::true_type {};

}  // namespace _drill_down

template <typename I>
using ValueType = typename std::iterator_traits<I>::value_type;

template <typename T>
using equivalent = typename decltype(_drill_down::equivalent<T>())::type;

template <typename I>
auto* drill_down(I _it) {
  using T = equivalent<ValueType<I>>;
  auto* res = &*_it;
  if constexpr (_drill_down::is_const_pointer<decltype(res)>{}) {
    return reinterpret_cast<const T*>(&*_it);
  } else {
    return reinterpret_cast<T*>(&*_it);
  }
}

template <typename I>
using equivalent_iterator = decltype(drill_down(std::declval<I>()));


template <typename I>
// require ContigiousIterator<I>
auto drill_down_range(I _f, I _l) {
  auto* f = drill_down(_f);
  auto* l = f + (_l - _f);

  return std::pair{f, l};
}

template <typename I, typename T>
I undo_drill_down(I _f, T* f) {
  return _f + (f - drill_down(_f));
}

template <typename T>
equivalent<T> equivalent_cast(const T& x) {
  equivalent<T> res;
  static_assert(sizeof(T) == sizeof(equivalent<T>));
  std::memcpy(&res, &x, sizeof(x)); // Just a bit_cast, evaporates
  return res;
}

template <typename T>
T from_equivalent_cast(const equivalent<T>& x) {
  T res;
  static_assert(sizeof(T) == sizeof(equivalent<T>));
  std::memcpy(&res, &x, sizeof(x)); // Just a bit_cast, evaporates
  return res;
}

}  // namespace unsq

#endif  // UNSQ_DRILL_DOWN_H_
