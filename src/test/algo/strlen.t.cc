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

#include "algo/strlen.h"

#include <string>

#include "test/catch.h"

namespace algo {
namespace {

TEST_CASE("algo.simd.strings.strlen", "[algo, simd, strings]") {
  REQUIRE(algo::strlen("a") == 1);
  REQUIRE(algo::strlen("") == 0);

  {
    std::string s(18, 'a');
    REQUIRE(algo::strlen(s.c_str()) == 18);
  }

  {
    std::string s(73, 'a');
    REQUIRE(algo::strlen(s.c_str()) == 73);
  }
}

}  // namespace
}  // namespace algo
