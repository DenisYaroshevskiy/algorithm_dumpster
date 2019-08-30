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

#include "algo/quadratic_sort.h"

#include "test/catch.h"

#include "test/algo/stable_sort_generic_test.h"

namespace algo {
namespace {

TEST_CASE("algorithm.bubble_sort_n", "[algorithm]") {
  stable_sort_quadratic_test([](auto f, auto l, auto... params) {
    algo::bubble_sort_n(f, std::distance(f, l), params...);
  });
}

}  // namespace
}  // namespace algo
