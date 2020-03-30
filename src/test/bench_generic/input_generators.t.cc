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

template <size_t size>
void uint_t_generate_sorted_vector_test() {
  std::array inputs = {0, 2, 4};
  auto src = [&, pos = 0]() mutable { return inputs[pos++]; };

  auto res = generate_sorted_vector<algo::uint_t<size>>(inputs.size(), src);
  REQUIRE(res == std::vector<algo::uint_t<size>>{0, 2, 4});
}

template <typename UintPair>
void uint_pair_generate_sorted_vector_test() {
  std::array inputs = {0, 1, 2, 3, 4, 5};
  auto src = [&, pos = 0]() mutable { return inputs[pos++]; };

  auto res = generate_sorted_vector<UintPair>(inputs.size() / 2, src);
  REQUIRE(res == std::vector{UintPair{0, 1}, UintPair{2, 3}, UintPair{4, 5}});
}

TEST_CASE("bench.input_generators.generate_sorted_vector", "[bench]") {
  {
    std::array inputs = {0, 5, 2, 2, 1};
    auto src = [&, pos = 0]() mutable { return inputs[pos++]; };

    auto res = generate_sorted_vector<int>(inputs.size(), src);
    REQUIRE(res == std::vector{0, 1, 2, 2, 5});
  }
  {
    std::array inputs = {0, 2, 4};
    auto src = [&, pos = 0]() mutable { return inputs[pos++]; };

    auto res = generate_sorted_vector<std_int64_t>(inputs.size(), src);

    REQUIRE(res == std::vector<std_int64_t>{0, 2, 4});
  }
  {
    std::array inputs = {0, 2};
    auto src = [&, pos = 0]() mutable { return inputs[pos++]; };

    auto res = generate_sorted_vector<fake_url>(inputs.size(), src);

    REQUIRE(res[0].data == "https://0.com");
    REQUIRE(res[1].data == "https://2.com");
  }
  {
    std::array inputs = {0, 2, 4};
    auto src = [&, pos = 0]() mutable { return inputs[pos++]; };

    auto res = generate_sorted_vector<noinline_int>(inputs.size(), src);

    REQUIRE(res[0].data == 0);
    REQUIRE(res[1].data == 2);
    REQUIRE(res[2].data == 4);
  }

  uint_t_generate_sorted_vector_test<8>();
  uint_t_generate_sorted_vector_test<16>();
  uint_t_generate_sorted_vector_test<32>();
  uint_t_generate_sorted_vector_test<64>();

  uint_pair_generate_sorted_vector_test<uint_std_pair32>();
  uint_pair_generate_sorted_vector_test<uint_std_pair64>();
  uint_pair_generate_sorted_vector_test<uint_tuple_pair32>();
  uint_pair_generate_sorted_vector_test<uint_tuple_pair64>();
}

TEST_CASE("bench.input_generators.generate_unique_sorted_vector", "[bench]") {
  {
    std::array inputs = {0, 5, 2, 2, 1};
    auto src = [&, pos = 0]() mutable { return inputs[pos++]; };

    auto res = generate_unique_sorted_vector<int>(inputs.size() - 1, src);
    REQUIRE(res == std::vector{0, 1, 2, 5});
  }
  {
    std::array inputs{1, 2, 2, 4, 10};
    auto src = [&, pos = 0]() mutable { return inputs[pos++]; };

    auto res = generate_unique_sorted_vector<double>(inputs.size() - 1, src);
    REQUIRE(res == std::vector{0.1, 0.25, 0.5, 1.0});
  }
}

TEST_CASE("bench.input_generators.nth_vector_permutation", "[bench]") {
  {
    auto ints = nth_vector_permutation<int>(10, 0);

    REQUIRE(ints.size() == 10u);
    REQUIRE(std::is_sorted(ints.begin(), ints.end()));

    ints = nth_vector_permutation<int>(10, 100);

    REQUIRE(ints.size() == 10u);
    REQUIRE(std::is_sorted(ints.begin(), ints.end(), std::greater<>{}));
  }

  {
    auto doubles = nth_vector_permutation<double>(10, 0);

    REQUIRE(doubles.size() == 10u);
    REQUIRE(std::is_sorted(doubles.begin(), doubles.end()));

    doubles = nth_vector_permutation<double>(10, 100);

    REQUIRE(doubles.size() == 10u);
    REQUIRE(std::is_sorted(doubles.begin(), doubles.end(), std::greater<>{}));
  }
}

TEST_CASE("bench.input_generators.shuffled_vector", "[bench]") {
  auto run = [](int percentage) {
    return shuffled_vector(
        100u, percentage, [](size_t size) { return sorted_vector<int>(size); });
  };

  {
    auto v = run(0);
    REQUIRE(std::is_sorted(v.begin(), v.end()));
  }
  {
    auto v = run(100);
    REQUIRE(std::is_sorted(v.begin(), v.end(), std::greater<>{}));
  }
  {
    auto v1 = run(25);
    auto v2 = run(75);
    std::reverse(v2.begin(), v2.end());

    REQUIRE(v1 == v2);
  }

  {
    for (int i = 0; i < 100; i += 5) {
      auto expected = run(i);
      for (int j = 0; j < 5; ++j) {
        auto actual = run(i);
        REQUIRE(expected == actual);
      }
    }
  }
}

TEST_CASE("bench.input_generators.vector_with_zeros", "[bench]") {
  auto run = [](int percentage) {
    return vector_with_zeroes<int>(100, percentage);
  };

  {
    auto test = run(0);
    REQUIRE(0 == std::count(test.begin(), test.end(), 0));
  }

  {
    auto test = run(50);
    REQUIRE(50 == std::count(test.begin(), test.end(), 0));
  }

  {
    auto test = run(100);
    REQUIRE(100 == std::count(test.begin(), test.end(), 0));
  }
}

}  // namespace
}  // namespace detail
}  // namespace bench
