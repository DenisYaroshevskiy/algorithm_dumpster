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

#include "algo/unroll.h"

#include "test/catch.h"

namespace algo {
namespace {

template <size_t N>
constexpr int add_n() {
  int r = 0;
  unroll<N>([&r]() mutable { ++r; });
  return r;
}

TEST_CASE("algorithm.unroll", "[algorithm]") {
  static_assert(add_n<0>() == 0);
  static_assert(add_n<1>() == 1);
  static_assert(add_n<5>() == 5);
}

}  // namespace
}  // namespace algo
