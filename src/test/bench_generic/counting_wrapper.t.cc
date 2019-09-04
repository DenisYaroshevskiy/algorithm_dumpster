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

#include "bench_generic/counting_wrapper.h"

#include <sstream>

#include "test/catch.h"

namespace bench {
namespace {

struct fake_absl_hasher {
  template <typename U>
  static fake_absl_hasher combine(fake_absl_hasher hs, const U&) {
    return hs;
  }
};

TEST_CASE("bench.counting_wrapper", "[bench]") {
  using T = counting_wrapper<int>;

  {
    T::clear();
    T x(1);

    T y(x), z(std::move(x));
    REQUIRE(T::copy == 1);
    REQUIRE(T::move == 1);
    REQUIRE(y.body == 1);
    REQUIRE(z.body == 1);

    x.body = 2;
    y = x;
    z = std::move(x);
    REQUIRE(T::copy == 2);
    REQUIRE(T::move == 2);
    REQUIRE(y.body == 2);
    REQUIRE(z.body == 2);
  }

  {
    T::clear();

    T x(1), y(2);

    REQUIRE(!(x == y));
    REQUIRE(x != y);
    REQUIRE(x < y);
    REQUIRE(y > x);
    REQUIRE(x <= y);
    REQUIRE(y >= x);

    REQUIRE(T::equal == 2);
    REQUIRE(T::less == 4);
  }

  {
    T::clear();
    AbslHashValue(fake_absl_hasher{}, T{});
    std::hash<T>{}(T{});

    REQUIRE(T::hash == 2);
  }
}

TEST_CASE("bench.counting_wrapper.json", "[bench]") {
  counting_wrapper<int>::clear();

  std::stringstream actual;

  counting_wrapper<int> x;
  auto y = x;

  counters_to_json_dict(actual);

  static constexpr std::string_view expected =  //
R"_({
  "copy": 1,
  "move": 0,
  "equal": 0,
  "less": 0,
  "hash": 0
})_";

  REQUIRE(expected == actual.str());
}

}  // namespace
}  // namespace bench
