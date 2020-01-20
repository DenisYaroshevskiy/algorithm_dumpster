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

#include "simd/register.h"

#include <array>
#include <type_traits>

#include "test/catch.h"

namespace simd {
namespace {

template <typename T>
void is_same_test(T, T) {}

TEST_CASE("simd.register.type", "[simd]") {
  is_same_test(register_i<128>{}, __m128i{});
  is_same_test(register_i<256>{}, __m256i{});
}

TEST_CASE("simd.register.sizes", "[simd]") {
  {
    STATIC_REQUIRE(bit_width<register_i<128>>() == 128);
    STATIC_REQUIRE(bit_width<register_i<256>>() == 256);
  }

  {
    STATIC_REQUIRE(byte_width<register_i<128>>() == 16);
    STATIC_REQUIRE(byte_width<register_i<256>>() == 32);
  }

  {
    STATIC_REQUIRE(alignment<register_i<128>>() == 16);
    STATIC_REQUIRE(alignment<register_i<256>>() == 32);
  }
}

TEST_CASE("simg.register.arrays", "[simd]") {
  is_same_test(corresponding_default_array<register_i<128>, 8>{},
               std::array<uint8_t, 16>{});

  is_same_test(corresponding_default_array<register_i<256>, 8>{},
               std::array<uint8_t, 32>{});
}

TEMPLATE_TEST_CASE("simd.register.no_sign.operations", "[simd]",  //
                   register_i<128>, register_i<256>) {
  using reg_t = TestType;

  using default_array_8 = corresponding_default_array<reg_t, 8>;

  alignas(alignment<reg_t>()) default_array_8 a, b;

  a.fill(1);
  b.fill(2);

  auto* a_casted = reinterpret_cast<reg_t*>(a.data());
  auto* b_casted = reinterpret_cast<reg_t*>(b.data());

  SECTION("load/store aligned") {
    reg_t loaded_a = load_s(a_casted);
    store_s(b_casted, loaded_a);
    REQUIRE(a == b);
  }
}

}  // namespace
}  // namespace simd
