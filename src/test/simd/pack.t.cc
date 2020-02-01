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

#include <array>

#include "simd/pack.h"
#include "test/catch.h"

namespace simd {
namespace {

template <typename T>
void is_same_test(T, T) {}

#define ALL_TEST_PACKS                                                        \
  (pack<std::int8_t, 16>), (pack<std::int8_t, 32>), (pack<std::uint8_t, 16>), \
      (pack<std::uint8_t, 32>), (pack<std::int16_t, 8>),                      \
      (pack<std::int16_t, 16>), (pack<std::uint16_t, 8>),                     \
      (pack<std::uint16_t, 16>), (pack<std::int32_t, 4>),                     \
      (pack<std::int32_t, 8>), (pack<std::uint32_t, 4>),                      \
      (pack<std::uint32_t, 8>), (pack<std::int64_t, 2>),                      \
      (pack<std::int64_t, 4>), (pack<std::uint64_t, 2>),                      \
      (pack<std::uint64_t, 4>), (pack<const int*, 2>), (pack<const int*, 4>), \
      (pack<uintptr_t, 2>), (pack<uintptr_t, 4>)

TEST_CASE("simd.pack.types.register_t", "[simd]") {
  is_same_test(mm::register_i<128>{}, register_t<pack<std::int8_t, 16>>{});
  is_same_test(mm::register_i<128>{}, register_t<pack<char, 16>>{});
  is_same_test(mm::register_i<128>{}, register_t<pack<std::uint8_t, 16>>{});

  is_same_test(mm::register_i<256>{}, register_t<pack<std::int8_t, 32>>{});
  is_same_test(mm::register_i<256>{}, register_t<pack<std::uint8_t, 32>>{});

  is_same_test(mm::register_i<256>{}, register_t<pack<std::int16_t, 16>>{});
  is_same_test(mm::register_i<256>{}, register_t<pack<std::uint16_t, 16>>{});

  is_same_test(mm::register_i<128>{}, register_t<pack<std::uint32_t, 4>>{});
  is_same_test(mm::register_i<256>{}, register_t<pack<std::uint32_t, 8>>{});

  is_same_test(mm::register_i<256>{}, register_t<pack<std::int64_t, 4>>{});
  is_same_test(mm::register_i<256>{}, register_t<pack<int*, 4>>{});
}

TEST_CASE("simd.pack.types.vbool_t", "[simd]") {
  is_same_test(pack<std::uint8_t, 16>{}, vbool_t<pack<std::int8_t, 16>>{});
  is_same_test(pack<std::uint8_t, 16>{}, vbool_t<pack<std::uint8_t, 16>>{});
  is_same_test(pack<std::uint16_t, 16>{}, vbool_t<pack<std::uint16_t, 16>>{});
  is_same_test(pack<std::uintptr_t, 4>{}, vbool_t<pack<const int*, 4>>{});
}

TEMPLATE_TEST_CASE("simd.pack.size/alignment", "[simd]", ALL_TEST_PACKS) {
  using pack_t = TestType;
  using reg_t = register_t<pack_t>;

  STATIC_REQUIRE(sizeof(pack_t) == mm::byte_width<reg_t>());
  STATIC_REQUIRE(alignof(pack_t) == mm::alignment<reg_t>());
}

TEMPLATE_TEST_CASE("simd.pack.basic", "[simd]", ALL_TEST_PACKS) {
  using pack_t = TestType;
  using scalar = scalar_t<pack_t>;
  constexpr size_t size = size_v<pack_t>;

  alignas(pack_t) std::array<scalar, size> a, b;

  a.fill((scalar)0);
  b.fill((scalar)1);

  SECTION("load/store aligned") {
    auto x = load<size>(a.data());
    store(b.data(), x);

    REQUIRE(a == b);
  }

  SECTION("equality") {
    auto x = load<size>(a.data());
    auto y = load<size>(b.data());

    REQUIRE(x == x);
    REQUIRE(x != y);
  }
}

}  // namespace
}  // namespace simd