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

#include "algo/uint_tuple.h"

#include <type_traits>

#include "test/catch.h"

namespace algo {
namespace {

template <typename T, size_t... N>
void storage_type_test() {
  STATIC_REQUIRE(std::is_same_v<T, typename uint_tuple<N...>::storage_type>);
}

template <typename T, size_t... is, typename... Values>
void check_at_all_impl(T t, std::index_sequence<is...>, Values... vs) {
  auto require = [](bool b) { REQUIRE(b); };
  (require(get_at<is>(t) == vs), ...);
}

template <size_t... sizes, typename... Values>
void check_at_all(uint_tuple<sizes...> t, Values... vs) {
  check_at_all_impl(t, std::make_index_sequence<sizeof...(sizes)>{}, vs...);
}

TEST_CASE("algorithm.uint_tuple.type", "[algorithm]") {
  storage_type_test<uint8_t, 1>();
  storage_type_test<uint8_t, 4, 4>();
  storage_type_test<uint8_t, 8>();

  storage_type_test<uint16_t, 8, 8>();
  storage_type_test<uint16_t, 16>();

  storage_type_test<uint32_t, 8, 8, 8>();
  storage_type_test<uint32_t, 32>();

  storage_type_test<uint64_t, 32, 16, 16>();
  storage_type_test<uint64_t, 64>();

#ifdef HAS_128_INTS
  storage_type_test<algo::uint_t<128>, 64, 64>();
#endif  // HAS_128_INTS
}

TEST_CASE("algorithm.uint_tuple.get_at.type", "[algorithm]") {
  const auto t = uint_tuple<8, 16, 32, 8>{};
  STATIC_REQUIRE(std::is_same_v<decltype(get_at<0>(t)), uint8_t>);
  STATIC_REQUIRE(std::is_same_v<decltype(get_at<1>(t)), uint16_t>);
  STATIC_REQUIRE(std::is_same_v<decltype(get_at<2>(t)), uint32_t>);
  STATIC_REQUIRE(std::is_same_v<decltype(get_at<3>(t)), uint8_t>);
}

TEST_CASE("algorithm.uint_tuple.get_at,set_at", "[algorithm]") {
  using tuple_t = uint_tuple<8, 16, 32, 4, 1, 2>;
  auto t = tuple_t{5u, 20u, 80u, 7u, 1u, 2u};
  check_at_all(t, 5u, 20u, 80u, 7u, 1u, 2u);

  static constexpr auto mx = std::numeric_limits<uint64_t>::max();
  t = tuple_t{mx, mx, mx, mx, mx, mx};
  check_at_all(t, std::numeric_limits<uint8_t>::max(),
               std::numeric_limits<uint16_t>::max(),
               std::numeric_limits<uint32_t>::max(), (1u << 4) - 1, 1u,
               (1u << 2) - 1);

  SECTION("constexpr") {
    auto sum = [](size_t a, size_t b, size_t c) {
      auto t = uint_tuple<16, 32, 16>{a, b, c};
      return get_at<0>(t) + get_at<1>(t) + get_at<2>(t);
    };
    STATIC_REQUIRE(sum(1, 2, 3) == 6);
    STATIC_REQUIRE(sum(8, 13, 21) == 42);
  }
}

TEST_CASE("algorithm.uint_tuple.cmp.regular", "[algorithm]") {
  using pair_t = uint_tuple<8, 8>;
  auto x = pair_t{0, 0};
  REQUIRE(x == x);
  REQUIRE(x <= x);
  REQUIRE(x >= x);
  REQUIRE_FALSE(x != x);
  REQUIRE_FALSE(x < x);
  REQUIRE_FALSE(x > x);

  auto y = x;
  REQUIRE(y == x);
  REQUIRE(y <= x);
  REQUIRE(y >= x);
  REQUIRE_FALSE(y != x);
  REQUIRE_FALSE(y < x);
  REQUIRE_FALSE(y > x);
  REQUIRE(x == y);
  REQUIRE(x <= y);
  REQUIRE(x >= y);
  REQUIRE_FALSE(x != y);
  REQUIRE_FALSE(x < y);
  REQUIRE_FALSE(x > y);
}

TEST_CASE("algorithm.uint_tuple.lexicographical_cmp", "[algorithm]") {
  using pair_t = uint_tuple<8, 8>;
  auto x = pair_t{0, 0};
  auto y = pair_t{0, 0};

  REQUIRE(x == y);

  // Increasing x, x == {1, 1}
  set_at<0>(x, 1);
  set_at<1>(x, 1);

  REQUIRE(x > y);

  // Increasing y, y == {2, 0}
  set_at<0>(y, 2);

  REQUIRE(x < y);

  // Decreasing y, y = {1, 0}
  set_at<0>(y, 1);

  REQUIRE(x > y);
}

}  // namespace
}  // namespace algo
