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

#include "algo/move.h"

#include <array>

#include "test/algo/zeroed_int.h"

#include "test/catch.h"

namespace algo {
namespace {

TEST_CASE("algorithm.move", "[algorithm]") {
  using test_arr = std::array<zeroed_int, 3>;

  auto input = [] {
    test_arr r;
    r[0] = zeroed_int(1);
    r[1] = zeroed_int(2);
    r[2] = zeroed_int(3);
    return r;
  };

  auto check_in_out = [](const auto& in, const auto& out) {
    for (const auto& x : in) REQUIRE(x == zeroed_int());

    for (int i = 0; i < static_cast<int>(out.size()); ++i)
      REQUIRE(zeroed_int(i + 1) == out[i]);
  };

  {
    test_arr in = input();
    test_arr out;

    auto lout = algo::move(in.begin(), in.end(), out.begin());
    REQUIRE(lout == out.end());

    check_in_out(in, out);
  }

  {
    test_arr in = input();
    test_arr out;

    auto [lin, lout] = algo::move_n(in.begin(), in.size(), out.begin());
    REQUIRE(lin == in.end());
    REQUIRE(lout == out.end());

    check_in_out(in, out);
  }

  {
    test_arr in = input();
    test_arr out;

    auto fout = algo::move_backward(in.begin(), in.end(), out.end());
    REQUIRE(fout == out.begin());

    check_in_out(in, out);
  }

  {
    test_arr in = input();
    test_arr out;

    auto [fin, fout] = algo::move_backward_n(in.end(), in.size(), out.end());
    REQUIRE(fin == in.begin());
    REQUIRE(fout == out.begin());

    check_in_out(in, out);
  }
}

}  // namespace
}  // namespace algo
