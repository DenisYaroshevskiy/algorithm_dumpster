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

#include "algo/half_nonnegative.h"

#include <numeric>

#include "test/catch.h"

namespace algo {
namespace {

struct Numeric {
  int data;

  /*implicit*/ constexpr Numeric(int x) : data(x) {}

  constexpr Numeric& operator/=(const Numeric& x) {
    data /= x.data;
    return *this;
  }

  friend constexpr Numeric operator/(const Numeric& x, const Numeric& y) {
    Numeric tmp = x;
    tmp /= y;
    return tmp;
  }

  friend constexpr bool operator==(const Numeric& x, const Numeric& y) {
    return x.data == y.data;
  }
};

template <typename N, typename UnderlyingN = N>
constexpr bool test(N max_v = N{std::numeric_limits<UnderlyingN>::max()}) {
  return algo::half_nonnegative(max_v) == max_v / 2;
}

TEST_CASE("algorithm.half_positve", "[algorithm]") {
  static_assert(test<char>());
  static_assert(test<int>());
  static_assert(test<long>());
  static_assert(test<size_t>());
  static_assert(test<Numeric, int>());
}

}  // namespace
}  // namespace algo
