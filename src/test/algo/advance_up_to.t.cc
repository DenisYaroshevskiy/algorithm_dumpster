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

#include "algo/advance_up_to.h"

#include <array>
#include <list>

#include "test/catch.h"

namespace algo {
namespace {

TEST_CASE("algorithm.advance_up_to.random_access", "[algorithm]") {
  static constexpr std::array data{1, 2, 3, 4, 5};

  constexpr auto test = [](const int* f, std::ptrdiff_t n,
                           const int* expected_f, std::ptrdiff_t expected_n) {
    advance_up_to(f, data.end(), n);
    return (expected_f == f) && (expected_n == n);
  };

  static_assert(test(data.begin(), 0, data.begin(), 0));
  static_assert(test(data.begin(), 1, data.begin() + 1, 0));

  static_assert(test(data.begin(), 6, data.end(), 1));
}

TEST_CASE("algorithm.advance_up_to.forward", "[algorithm]") {
  std::list data{1, 2, 3, 4, 5};
  using I = std::list<int>::const_iterator;

  auto test = [&](I f, DifferenceType<I> n, I expected_f,
                 DifferenceType<I> expected_n) {
    advance_up_to(f, data.cend(), n);

    REQUIRE(expected_f == f);
    REQUIRE(expected_n == n);
  };

  test(data.begin(), 0, data.begin(), 0);
  test(data.begin(), 1, std::next(data.begin()), 0);
  test(data.begin(), 6, data.end(), 1);
}

}  // namespace
}  // namespace algo
