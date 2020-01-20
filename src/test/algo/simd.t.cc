/*
 * Copyright 2020 Denis Yaroshevskiy
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

#include "algo/simd.h"

#include <algorithm>
#include <array>

#include <iostream>

#include "test/catch.h"

namespace algo {
namespace {

TEST_CASE("algo.simd.set/load/equal", "[algo, simd]") {
  simd<std::int8_t, 16> x, y;

  x.fill(5);

  alignas(16) std::array<std::int8_t, 16> input;
  input.fill(5);
  y.load(input.data());

  REQUIRE(x == y);

  x.fill(6);
  REQUIRE(x != y);
}

TEST_CASE("algo.simd.pick_min/pick_max", "[algo, simd]") {
  simd<std::int8_t, 16> x, y;

  x.fill(5);
  y.fill(6);

  REQUIRE(x == pick_min(x, y));
  REQUIRE(y == pick_max(x, y));
}

TEST_CASE("algo.simd.blend_n_from_high/from_low", "[algo, simd]") {
  simd<std::int8_t, 16> x, y;

  x.fill(5);
  y.fill(6);

  alignas(16) std::array<std::int8_t, 16> expected;
  alignas(16) std::array<std::int8_t, 16> actual;

  expected.fill(5);

  for (size_t i = 0;; ++i) {
    blend_n_from_high(x, y, i).store(actual.data());
    REQUIRE(expected == actual);

    if (i == 16) break;
    expected[i] = 6;
  }

  expected.fill(5);

  for (size_t i = 0;; ++i) {
    blend_n_from_low(x, y, i).store(actual.data());
    REQUIRE(expected == actual);

    if (i == 16) break;
    expected[15 - i] = 6;
  }

  // Double check special cases:
  expected.fill(6);
  blend_n_from_low(x, y, 16).store(actual.data());
  REQUIRE(expected == actual);
  blend_n_from_high(x, y, 16).store(actual.data());
  REQUIRE(expected == actual);

  expected.fill(5);
  blend_n_from_low(x, y, 0).store(actual.data());
  REQUIRE(expected == actual);
  blend_n_from_high(x, y, 0).store(actual.data());
  REQUIRE(expected == actual);
}

TEST_CASE("algo.simd.load_unaligned_with_filler", "[algo, simd]") {
  alignas(16) std::array<std::int8_t, 32> in;
  in.fill(5);

  std::array<std::int8_t, 16> expected;
  expected.fill(5);

  std::array<std::int8_t, 16> actual;

  auto do_load_at = [&](std::size_t i) {
    load_unaligned_with_filler<simd<std::int8_t, 16>>(&in[i], 6).store(
        actual.data());
  };

  do_load_at(0);
  REQUIRE(expected == actual);

  do_load_at(1);
  expected[0] = 6;
  REQUIRE(expected == actual);

  expected.fill(5);

  for (std::size_t i = 0; i != 16; ++i) {
    do_load_at(i);
    REQUIRE(expected == actual);
    expected[i] = 6;
  }
}

TEST_CASE("algo.simd.have_pairwise_equal_elements", "[algo, simd]") {
  simd<std::int8_t, 16> x, y;

  alignas(16) std::array<std::int8_t, 16> x_arr;
  alignas(16) std::array<std::int8_t, 16> y_arr;

  auto run = [&] {
    x.load(x_arr.data());
    y.load(y_arr.data());
    return any_pairwise_equal(x, y);
  };

  x_arr.fill(0);
  y_arr.fill(1);

  REQUIRE(!run());

  y_arr[3] = 0;

  REQUIRE(run());
}

TEST_CASE("algo.simd.first_pairwise_equal", "[algo, simd]") {
   simd<std::int8_t, 16> x, y;

  alignas(16) std::array<std::int8_t, 16> x_arr;
  alignas(16) std::array<std::int8_t, 16> y_arr;

  auto run = [&] {
    x.load(x_arr.data());
    y.load(y_arr.data());
    return first_pairwise_equal(x, y);
  };

  x_arr.fill(0);
  y_arr.fill(0);

  for (size_t i = 0; i != 16; ++i) {
    REQUIRE(run() == i);
    y_arr[i] = 1;
  }
}

}  // namespace
}  // namespace algo
