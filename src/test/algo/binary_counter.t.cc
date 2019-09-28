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

#include "algo/binary_counter.h"

#include <random>

#include "test/catch.h"

#include "test/algo/stability_test_util.h"

namespace algo {
namespace {

struct stabitlity_test_op {
  stable_unique operator()(stable_unique x, stable_unique y) const {
    REQUIRE(x.second < y.second);
    return x;
  }
};

struct stable_zero_t {
  operator stable_unique() const { return {zeroed_int(0), 0}; }

  friend bool operator==(const stable_unique& x, const stable_zero_t&) {
    return x.first.body == 0;
  }

  friend bool operator!=(const stable_unique& x, const stable_zero_t& y) {
    return !(x == y);
  }
};

template <typename Bc>
std::vector<int> to_ints(const Bc& bc) {
  auto digits = bc.significant_digits();
  std::vector<int> res(digits.second - digits.first);
  std::transform(digits.first, digits.second, res.begin(),
                 [](const stable_unique& x) { return x.first.body; });
  return res;
}

template <typename Bc>
void count_to_5(Bc bc) {
  REQUIRE(to_ints(bc) == std::vector<int>{});
  bc.add({zeroed_int(1), 1});
  REQUIRE(to_ints(bc) == std::vector{1});
  bc.add({zeroed_int(2), 2});
  REQUIRE(to_ints(bc) == std::vector{0, 1});
  bc.add({zeroed_int(3), 3});
  REQUIRE(to_ints(bc) == std::vector{3, 1});
  bc.add({zeroed_int(4), 4});
  REQUIRE(to_ints(bc) == std::vector{0, 0, 1});
  bc.add({zeroed_int(5), 5});

  REQUIRE(bc.reduce().first.body == 1);
}

template <template <typename...> class Bc>
void stability_test() {
  using counter = Bc<stabitlity_test_op, stable_unique, stable_zero_t>;

  {
    counter bc{stabitlity_test_op{}, stable_zero_t{}};
    count_to_5(std::move(bc));
  }

  {
    counter bc{stabitlity_test_op{}, stable_zero_t{}};
    bc.reserve(2);
    count_to_5(std::move(bc));
  }
}

struct min {
  int operator()(int x, int y) { return y < x ? y : x; }
};

template <template <typename...> class Bc>
void min_test() {
  using counter = Bc<min>;

  auto run = [](counter bc) {
    int expected = 1;

    for (auto x : {5, 2, 7, 1, 4}) {
      bc.add(x);
    }

    REQUIRE(expected == bc.reduce());
  };

  counter bc{min{}, -1};
  run(bc);

  bc.reserve(3);
  run(bc);
}

template <template <typename...> class Bc>
void binary_counter_test() {
  stability_test<Bc>();
  min_test<Bc>();
}

template <typename... Args>
struct binary_counter_to_16 : binary_counter_fixed<4, Args...> {
  using binary_counter_fixed<4, Args...>::binary_counter_fixed;
};

TEST_CASE("algorithm.binary_counter_fixed", "[algorithm]") {
  binary_counter_test<binary_counter_to_16>();
}

}  // namespace
}  // namespace algo
