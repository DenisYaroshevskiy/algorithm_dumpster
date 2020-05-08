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

#include "unsq/drill_down.h"

#include <type_traits>
#include <vector>

#include "test/catch.h"

namespace unsq {
namespace {

template <typename Expected, typename U>
void pointer_type_test(U* x) {
  STATIC_REQUIRE(std::is_same_v<Expected, decltype(x)>);
}



TEST_CASE("unsq drill down, equivalent", "[simd]") {
  STATIC_REQUIRE(std::is_same_v<char, equivalent<char>>);
  STATIC_REQUIRE(std::is_same_v<int, equivalent<int>>);
  STATIC_REQUIRE(std::is_same_v<unsigned, equivalent<unsigned>>);

  struct S {
    std::int16_t _;
  };
  STATIC_REQUIRE(std::is_same_v<std::uint16_t, equivalent<S>>);
}

TEST_CASE("unsq drill down", "[simd]") {
  struct S {
    std::int16_t _;
  };
  std::vector<S> data { S{0} };

  pointer_type_test<std::uint16_t*>(drill_down(&data[0]));
  pointer_type_test<const std::uint16_t*>(drill_down(&*data.cbegin()));
  pointer_type_test<std::uint16_t*>(drill_down(data.begin()));
  pointer_type_test<const std::uint16_t*>(drill_down(data.cbegin()));
}

TEST_CASE("unsq equivalent cast", "[simd]")  {
  struct S {
    std::uint16_t x;
  } test {123};

  STATIC_REQUIRE(std::is_same_v<std::uint16_t, equivalent<S>>);

  std::uint16_t down = equivalent_cast(test);
  REQUIRE(down == std::uint16_t{123});

  S up = from_equivalent_cast<S>(down);
  REQUIRE(up.x == test.x);
}

}  // namespace
}  // namespace unsq
