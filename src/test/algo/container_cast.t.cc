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

#include "algo/container_cast.h"

#include <array>
#include <list>
#include <set>
#include <unordered_set>
#include <vector>

#include "test/catch.h"

#include "zeroed_int.h"

namespace algo {
namespace {

TEST_CASE("algorithm.container_cast.meta_programming", "[algorithm]") {
  {
    std::vector<int> in;
    auto out = container_cast<std::list>(in);
    static_assert(std::is_same_v<decltype(out), std::list<int>>);
  }

  {
    std::vector<char> in;
    auto out = container_cast<std::list>(in);
    static_assert(std::is_same_v<decltype(out), std::list<char>>);
  }

  {
    std::list<int> in;
    auto out = container_cast<std::vector>(in);
    static_assert(std::is_same_v<decltype(out), std::vector<int>>);
  }

  {
    std::vector<int> in;
    auto out = container_cast<std::set>(in);
    static_assert(std::is_same_v<decltype(out), std::set<int>>);
  }

  {
    std::vector<int> in;
    auto out = container_cast<std::unordered_multiset>(in);
    static_assert(std::is_same_v<decltype(out), std::unordered_multiset<int>>);
  }
}

TEST_CASE("algorithm.container_cast.values", "[algorithm]") {
  const std::array ints = {1, 2, 3};
  {
    auto out = container_cast<std::vector>(ints);
    REQUIRE(out == std::vector{1, 2, 3});
  }
  {
    std::vector<zeroed_int> in(ints.begin(), ints.end());
    std::vector<zeroed_int> out = container_cast<std::vector>(std::move(in));

    REQUIRE(in == std::vector<zeroed_int>(3));
    REQUIRE(out == std::vector<zeroed_int>(ints.begin(), ints.end()));
  }
}

}  // namespace
}  // namespace algo
