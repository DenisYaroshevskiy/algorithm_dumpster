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
#include <vector>

#include "test/catch.h"

#include "algo/comparisons.h"
#include "algo/copy.h"
#include "algo/make_vector_of_iterators.h"

namespace algo {
namespace {

template <typename Alg>
void apply_rearrangement_test_one_input(const std::vector<int>& in, Alg alg) {
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

template <typename Alg>
void apply_rearrangement_test(Alg alg) {
  std::vector<int> in;
  int i = 0;
  do {
    apply_rearrangement_test_one_input(in, alg);
    in.push_back(++i);
  } while (i < 8);
}

TEST_CASE("algorithm.apply_rearrangment_copy", "[algorithm]") {
  apply_rearrangement_test([](auto positions, auto f, auto) {
    std::vector<int> res(positions.size());
    apply_rearrangment_copy(positions.begin(), positions.end(), res.begin());
    algo::copy(res.begin(), res.end(), f);
  });
}

}  // namespace
}  // namespace algo
