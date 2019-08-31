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

#include "algo/apply_rearrangment.h"

#include <algorithm>
#include <random>
#include <vector>

#include "test/catch.h"

#include "algo/comparisons.h"
#include "algo/copy.h"
#include "algo/make_vector_of_iterators.h"
#include "algo/move.h"
#include "test/algo/zeroed_int.h"

namespace algo {
namespace {

using test_t = zeroed_int_regular;

template <typename Alg>
void apply_rearrangement_test_all_permutations(const std::vector<test_t>& in,
                                               Alg alg) {
  auto expected = in;
  auto actual = in;
  auto positions = make_vector_of_iterators(actual.begin(), actual.end());

  while (true) {
    algo::copy(in.begin(), in.end(), actual.begin());
    auto positions_copy = positions;

    alg(positions_copy, actual.begin(), actual.end());

    REQUIRE(expected == actual);

    if (!std::next_permutation(expected.begin(), expected.end())) break;
    // positions are sorted the same as ints
    std::next_permutation(positions.begin(), positions.end());
  }
}

class apply_rearrangement_test_random_permutation {
  std::mt19937 g1, g2;

 public:
  template <typename Alg>
  void operator()(const std::vector<test_t>& in, Alg alg) {
    std::vector<test_t> expected = in;
    std::shuffle(expected.begin(), expected.end(), g1);

    std::vector<test_t> actual = in;
    auto positions = make_vector_of_iterators(actual.begin(), actual.end());
    std::shuffle(positions.begin(), positions.end(), g2);

    alg(positions, actual.begin(), actual.end());

    REQUIRE(expected == actual);
  }
};

template <typename Alg>
void apply_rearrangement_test(Alg alg) {
  std::vector<test_t> in;
  int i = 0;
  do {
    apply_rearrangement_test_all_permutations(in, alg);
    in.emplace_back(++i);
  } while (i < 8);

  {
    auto iota_vec = [](size_t size) {
      std::vector<test_t> res(size);
      int i = 0;
      std::generate(res.begin(), res.end(),
                    [&]() mutable { return test_t{++i}; });
      return res;
    };

    apply_rearrangement_test_random_permutation run_random_test;

    for (size_t i : {1000u, 1234u, 8832u}) {
      run_random_test(iota_vec(i), alg);
    }
  }
}

TEST_CASE("algorithm.apply_rearrangment_copy", "[algorithm]") {
  apply_rearrangement_test([](auto positions, auto f, auto) {
    std::vector<test_t> res(positions.size());
    apply_rearrangment_copy(positions.begin(), positions.end(), res.begin());
    algo::copy(res.begin(), res.end(), f);
  });
}

TEST_CASE("algorithm.apply_rearrangment_move", "[algorithm]") {
  apply_rearrangement_test([](auto positions, auto f, auto l) {
    std::vector<test_t> res(positions.size());
    apply_rearrangment_move(positions.begin(), positions.end(), res.begin());
    std::for_each(f, l, [](const test_t& x) { REQUIRE(x == test_t(0)); });
    algo::move(res.begin(), res.end(), f);
  });
}

TEST_CASE("algorithm.apply_rearrangment", "[algorithm]") {
  apply_rearrangement_test([](auto positions, auto f, auto l) {
    apply_rearrangment(positions.begin(), positions.end(), f, l);
  });
}

TEST_CASE("algorithm.apply_rearrangment_no_marker", "[algorithm]") {
  apply_rearrangement_test([](auto positions, auto f, auto) {
    apply_rearrangment_no_marker(positions.begin(), positions.end(), f);
  });
}

}  // namespace
}  // namespace algo
