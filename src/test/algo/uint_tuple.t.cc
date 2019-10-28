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
  auto t = uint_tuple<8, 16, 32, 8>{};
  algo::set_at<0>(t, 5);
  algo::set_at<1>(t, 20);
  algo::set_at<2>(t, 80);
  algo::set_at<3>(t, std::numeric_limits<uint8_t>::max());
  REQUIRE(algo::get_at<0>(t) == 5);
  REQUIRE(algo::get_at<1>(t) == 20);
  REQUIRE(algo::get_at<2>(t) == 80);
  REQUIRE(algo::get_at<3>(t) == std::numeric_limits<uint8_t>::max());
}

TEST_CASE("algorithm.uint_tuple.tuple_element", "[algorithm]") {
  using tuple_t = uint_tuple<8, 16, 32, 8>;
  STATIC_REQUIRE(std::is_same_v<std::tuple_element_t<0, tuple_t>, uint8_t>);
  STATIC_REQUIRE(std::is_same_v<std::tuple_element_t<1, tuple_t>, uint16_t>);
  STATIC_REQUIRE(std::is_same_v<std::tuple_element_t<2, tuple_t>, uint32_t>);
  STATIC_REQUIRE(std::is_same_v<std::tuple_element_t<3, tuple_t>, uint8_t>);

  STATIC_REQUIRE(
      std::is_same_v<std::tuple_element_t<0, const tuple_t>, const uint8_t>);
  STATIC_REQUIRE(std::is_same_v<std::tuple_element_t<1, volatile tuple_t>,
                                volatile uint16_t>);
  STATIC_REQUIRE(std::is_same_v<std::tuple_element_t<2, const volatile tuple_t>,
                                const volatile uint32_t>);
}

TEST_CASE("algorithm.uint_tuple.tuple_size", "[algorithm]") {
  using tuple_t = uint_tuple<8, 16, 32, 8>;
  STATIC_REQUIRE(std::tuple_size_v<tuple_t> == 4);
  STATIC_REQUIRE(std::tuple_size_v<uint_tuple<>> == 0);
}

}  // namespace
}  // namespace algo
