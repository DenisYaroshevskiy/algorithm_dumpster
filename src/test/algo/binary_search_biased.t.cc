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

#include "algo/binary_search_biased.h"

#include "test/catch.h"

#include "test/algo/binary_search_generic_test.h"

namespace algo {
namespace {

TEST_CASE("algorithm.binary_search.lower_bound_biased_expensive_cmp",
          "[algorithm]") {
  test_lower_bound([](auto f, auto l, const auto& v) {
    return algo::lower_bound_biased_expensive_cmp(f, l, v);
  });
}

TEST_CASE("algorithm.binary_search.lower_bound_biased", "[algorithm]") {
  test_lower_bound([](auto f, auto l, const auto& v) {
    return algo::lower_bound_biased(f, l, v);
  });
}

TEST_CASE("algorithm.binary_search.lower_bound_linear", "[algorithm]") {
  test_lower_bound([](auto f, auto l, const auto& v) {
    return algo::lower_bound_linear(f, l, v);
  });
}

TEST_CASE("algorithm.binary_search.lower_bound_hinted", "[algorithm]") {
  test_lower_bound_hinted([](auto f, auto h, auto l, const auto& v) {
    return algo::lower_bound_hinted(f, h, l, v);
  });
}

}  // namespace
}  // namespace algo
