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

#include "algo/make_vector_of_iterators.h"

#include <list>
#include <vector>

#include "test/catch.h"

namespace algo {
namespace {

TEST_CASE("algorithm.make_vector_of_iterators", "[algorithm]") {
  auto run_test = [](auto f, auto l) {
    auto actual = make_vector_of_iterators(f, l);
    auto res_f = actual.begin();

    while (f != l) {
      REQUIRE(f == *res_f);
      ++f;
      ++res_f;
    }

    REQUIRE(res_f == actual.end());
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
