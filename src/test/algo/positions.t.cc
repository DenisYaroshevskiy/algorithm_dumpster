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

#include "algo/positions.h"

#include <iostream>
#include <list>
#include <vector>

#include "test/catch.h"

namespace algo {
namespace {

TEST_CASE("algorithm.lift_as_vector", "[algorithm]") {
  auto run_test = [](auto f, auto l) {
    const auto [actual, lifted_f, lifted_l] = lift_as_vector(f, l);

    using lifted_I = typename decltype(actual)::value_type;

    static_assert(ForwardIterator<lifted_I>);
    static_assert(BidirectionalIterator<lifted_I>);

    REQUIRE(actual.size() == std::distance(f, l));

    {
      auto res_f = actual.begin();
      auto cur_res = actual.begin();
      auto cur_in = f;
      int n = 0;

      while (cur_in != l) {
        REQUIRE(&*cur_in == &**cur_res);
        REQUIRE(*cur_res - *res_f == n);
        ++cur_in;
        ++cur_res;
        ++n;
      }

      REQUIRE(cur_res == actual.end());
    }

    if (actual.size() >= 2) {
      auto rit = *(--actual.end());
      REQUIRE(*rit-- == *std::prev(l));
      REQUIRE(*rit == *std::prev(l, 2));
      --rit;
      REQUIRE(*rit == *std::prev(l, 3));

      REQUIRE(rit - actual[0] == actual.size() - 3);

      REQUIRE(*rit++ == *std::prev(l, 3));
      REQUIRE(*rit == *std::prev(l, 2));
    }

    { REQUIRE(std::is_sorted(actual.begin(), actual.end())); }

    if (actual.size() >= 1) {
      REQUIRE(lifted_f == actual[0]);
    }

    { REQUIRE(actual.size() == lifted_l - lifted_f); }

    if (actual.size() >= 2) {
      auto x = actual[0];
      auto y = actual[1];

      REQUIRE(x < y);
      REQUIRE(y > x);
      REQUIRE(x == x);
      REQUIRE(x != y);
    }

    if (actual.size() > 1) {
      REQUIRE(lifted_l - actual[0] == actual.size());
    }
  };

  {
    std::vector<int> v;
    run_test(v.begin(), v.end());

    v.resize(5);
    run_test(v.begin(), v.end());
    run_test(&v[0], &v[0] + v.size());
  }

  {
    std::list<int> l;
    run_test(l.begin(), l.end());

    for (int i = 0; i < 5; ++i) {
      l.push_back(i);
    }
    run_test(l.begin(), l.end());
  }
}

}  // namespace
}  // namespace algo
