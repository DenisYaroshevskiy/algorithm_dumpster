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

#include "simd/mm.h"

#include <array>
#include <type_traits>

#include "test/catch.h"

namespace mm {
namespace {

template <typename T>
void is_same_test(T, T) {}

template <typename Register, typename T>
using type_array = std::array<T, byte_width<Register>() / sizeof(T)>;

template <typename Register>
using byte_array = type_array<Register, std::byte>;

// Workaroud for catch template product.

template <typename T>
struct test_128 {
  static constexpr size_t width = 128;
  using scalar_t = T;
};

template <typename T>
struct test_256 {
  static constexpr size_t width = 256;
  using scalar_t = T;
};

TEST_CASE("simd.register.type", "[simd]") {
  is_same_test(register_i<128>{}, __m128i{});
  is_same_test(register_i<256>{}, __m256i{});
}

TEST_CASE("simd.register.sizes", "[simd]") {
  {
    STATIC_REQUIRE(bit_width<register_i<128>>() == 128);
    STATIC_REQUIRE(bit_width<register_i<256>>() == 256);
    STATIC_REQUIRE(bit_width<register_i<512>>() == 512);
  }

  {
    STATIC_REQUIRE(byte_width<register_i<128>>() == 16);
    STATIC_REQUIRE(byte_width<register_i<256>>() == 32);
    STATIC_REQUIRE(byte_width<register_i<512>>() == 64);
  }

  {
    STATIC_REQUIRE(alignment<register_i<128>>() == 16);
    STATIC_REQUIRE(alignment<register_i<256>>() == 32);
    STATIC_REQUIRE(byte_width<register_i<512>>() == 64);
  }
}

TEMPLATE_TEST_CASE("simd.register.just_bytes", "[simd]",  //
                   register_i<128>, register_i<256>) {
  using reg_t = TestType;

  alignas(alignment<reg_t>()) byte_array<reg_t> a, b;

  a.fill(std::byte{1});
  b.fill(std::byte{2});

  auto* a_casted = reinterpret_cast<reg_t*>(a.data());
  auto* b_casted = reinterpret_cast<reg_t*>(b.data());

  SECTION("load/store") {
    reg_t loaded_a = load(a_casted);
    store(b_casted, loaded_a);
    REQUIRE(a == b);
  }

  SECTION("setzero") {
    a.fill(std::byte{0});

    reg_t to_store = setzero<bit_width<reg_t>()>();
    store(b_casted, to_store);

    REQUIRE(a == b);
  }
}

TEMPLATE_PRODUCT_TEST_CASE("simd.register.ints", "[simd]",    //
                           (test_128, test_256),              //
                           (std::int8_t, std::int16_t,        //
                            std::int32_t, std::int64_t,       //
                            std::uint8_t, std::uint16_t,      //
                            std::uint32_t, std::uint64_t)) {  //
  using reg_t = register_i<TestType::width>;
  using scalar_t = typename TestType::scalar_t;

  alignas(alignment<reg_t>()) type_array<reg_t, scalar_t> a, b, c;

  a.fill(scalar_t{1});
  b.fill(scalar_t{2});

  auto* a_casted = reinterpret_cast<reg_t*>(a.data());
  auto* b_casted = reinterpret_cast<reg_t*>(b.data());
  auto* c_casted = reinterpret_cast<reg_t*>(c.data());

  (void)a_casted;

  SECTION("set1") {
    reg_t filled = set1<bit_width<reg_t>()>(scalar_t{1});
    store(b_casted, filled);
    REQUIRE(a == b);
  }

  SECTION("min/max") {
    if constexpr (sizeof(scalar_t) < 8) {
      scalar_t small = -1;
      scalar_t large = 1;
      if (!std::is_signed_v<scalar_t>) {
        std::swap(small, large);
      }

      for (size_t i = 0; i != a.size(); i += 2) {
        a[i] = small;
        a[i + 1] = large;
        b[i] = large;
        b[i + 1] = small;
      }

      reg_t x = load(a_casted);
      reg_t y = load(b_casted);
      reg_t min_v = min<scalar_t>(x, y);
      reg_t max_v = max<scalar_t>(x, y);

      a.fill(small);
      b.fill(large);

      store(c_casted, min_v);
      REQUIRE(a == c);

      store(c_casted, max_v);
      REQUIRE(b == c);
    }
  }
}

}  // namespace
}  // namespace mm