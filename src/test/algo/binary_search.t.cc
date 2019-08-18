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

#include "algo/binary_search.h"

#include "test/catch.h"

#include "test/algo/binary_search_generic_test.h"

namespace algo {
namespace {

TEST_CASE("algorithm.binary_search.lower_bound", "[algorithm]") {
  test_lower_bound(
      [](auto f, auto l, const auto& v) { return algo::lower_bound(f, l, v); });
}

TEST_CASE("algorithm.binary_search.lower_bound_n", "[algorithm]") {
  test_lower_bound_n([](auto f, auto n, const auto& v) {
    return algo::lower_bound_n(f, n, v);
  });
}

}  // namespace
}  // namespace algo
