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

#include "simd/bits.h"

#include "test/catch.h"

namespace simd {

TEST_CASE("lsb_less", "[simd]") {
  REQUIRE_FALSE(lsb_less(0u, 0u));
  REQUIRE_FALSE(lsb_less(1u, 1u));
  REQUIRE(lsb_less(0u, 1u));  // 0000 0001
  REQUIRE(lsb_less(2u, 1u));  // 0010 0001
  REQUIRE(lsb_less(1u, 3u));  // 0001 0011
  REQUIRE(lsb_less(4u, 1u));  // 0100 0001
  REQUIRE(lsb_less(1u, 5u));  // 0100 0001
  REQUIRE(lsb_less(5u, 3u));  // 0101 0011
}

TEST_CASE("set_lower_n_bits", "[simd]") {
  REQUIRE(0 == set_lower_n_bits(0));
  REQUIRE(1 == set_lower_n_bits(1));
  REQUIRE(3 == set_lower_n_bits(2));
  REQUIRE(0xffffffff == set_lower_n_bits(32));
}

}  // namespace simd
