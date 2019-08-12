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

#include "bench_generic/input_generators.h"

#include <array>

#include "test/catch.h"

namespace bench {
namespace detail {
namespace {

TEST_CASE("bench.input_generators.generate_random_vector", "[bench]") {
    std::array inputs = {0, 5, 2, 2, 1};
    auto src = [&, pos = 0]() mutable { return inputs[pos++]; };

    auto res = generate_random_vector<int>(inputs.size(), src);
    REQUIRE(res == std::vector(inputs.begin(), inputs.end()));
}

TEST_CASE("bench.input_generators.generate_sorted_vector", "[bench]") {
  std::array inputs = {0, 5, 2, 2, 1};
  auto src = [&, pos = 0]() mutable { return inputs[pos++]; };

  auto res = generate_sorted_vector<int>(inputs.size(), src);
  REQUIRE(res == std::vector{0, 1, 2, 2, 5});
}

TEST_CASE("bench.input_generators.generate_unique_sorted_vector", "[bench]") {
  std::array inputs = {0, 5, 2, 2, 1};
  auto src = [&, pos = 0]() mutable { return inputs[pos++]; };

  auto res = generate_unique_sorted_vector<int>(inputs.size() - 1, src);
  REQUIRE(res == std::vector{0, 1, 2, 5});
}

}  // namespace
}  // namespace detail
}  // namespace bench
