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

#include "algo/shuffle_biased.h"

#include <algorithm>
#include <numeric>
#include <random>
#include <iostream>

#include "test/catch.h"

namespace algo {

TEST_CASE("algorithm.shuffle_biased", "[algorithm]") {
  const std::vector<int> in = []{
    std::vector<int> r(100);
    std::iota(r.begin(), r.end(), 0);
    return r;
  }();

  auto std_shuffle = in;
  std::shuffle(std_shuffle.begin(), std_shuffle.end(), std::mt19937{});

  {
    auto v = in;
    algo::shuffle_biased(v.begin(), v.end(), 1, std::mt19937{});
    REQUIRE(v == in);
  }

  {
    auto v = in;
    algo::shuffle_biased(v.begin(), v.end(), 15, std::mt19937{});
    REQUIRE(std::is_permutation(v.begin(), v.end(), in.begin()));
    REQUIRE(v != std_shuffle);
  }

  {
    auto v = in;
    algo::shuffle_biased(v.begin(), v.end(), v.size(), std::mt19937{});
    REQUIRE(v == std_shuffle);
  }
}

}  // namespace algo
