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

#include "simd/pack.h"

#include <algorithm>
#include <array>
#include <numeric>

#include <iostream>

#include "test/catch.h"

namespace simd {
namespace {

template <typename T>
void is_same_test(T, T) {}

// clang-format off
#define ALL_TEST_PACKS                                 \
  (pack<std::int8_t, 16>),  (pack<std::int8_t, 32>),   \
  (pack<std::uint8_t, 16>), (pack<std::uint8_t, 32>),  \
  (pack<std::int16_t, 8>),  (pack<std::int16_t, 16>),  \
  (pack<std::uint16_t, 8>), (pack<std::uint16_t, 16>), \
  (pack<std::int32_t, 4>),  (pack<std::int32_t, 8>),   \
  (pack<std::uint32_t, 4>), (pack<std::uint32_t, 8>),  \
  (pack<std::int64_t, 2>),  (pack<std::int64_t, 4>),   \
  (pack<std::uint64_t, 2>), (pack<std::uint64_t, 4>),  \
  (pack<const int*, 2>),    (pack<const int*, 4>)
// clang-format on

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
  is_same_test(pack<std::uint64_t, 4>{}, vbool_t<pack<const int*, 4>>{});
}

TEMPLATE_TEST_CASE("simd.pack.size/alignment", "[simd]", ALL_TEST_PACKS) {
  using pack_t = TestType;
  using reg_t = register_t<pack_t>;

  STATIC_REQUIRE(sizeof(pack_t) == mm::byte_width<reg_t>());
  STATIC_REQUIRE(alignof(pack_t) == mm::alignment<reg_t>());
}

TEMPLATE_TEST_CASE("simd.pack.totally_ordered", "[simd]", ALL_TEST_PACKS) {
  using pack_t = TestType;
  using scalar = scalar_t<pack_t>;
  constexpr size_t size = size_v<pack_t>;

  alignas(pack_t) std::array<scalar, size> a, b;

  const scalar small_v = (scalar)0;
  const scalar big_v = (scalar)(1);

  a.fill(small_v);
  b.fill(big_v);

  SECTION("load/store aligned") {
    auto x = load<pack_t>(a.data());
    store(b.data(), x);

    REQUIRE(a == b);
  }

  SECTION("load_unaligned") {
    std::array<scalar, size * 2> in;
    std::iota(in.begin(), in.end(), (scalar)0);

    for (std::uint8_t i = 0; i < size; ++i) {
      auto x = load_unaligned<pack_t>(&in[i]);
      std::copy_n(&in[i], size, a.begin());
      store(b.data(), x);
      REQUIRE(a == b);
    }
  }

  SECTION("load, previously aligned") {
    auto run = [&](std::ptrdiff_t offset) {
      auto* ptr = simd::previous_aligned_address<pack_t>(a.data() + offset);
      auto x = load<pack_t>(ptr);
      store(b.data(), x);
      REQUIRE(a == b);
      REQUIRE(a.data() == ptr);
    };

    run(0);
    run(1);
    if (a.size() > 5) {
      run(5);
    }
  }

  SECTION("equality") {
    auto x = load<pack_t>(a.data());
    auto y = load<pack_t>(b.data());

    REQUIRE(x == x);
    REQUIRE(x != y);
  }

  SECTION("less/greater") {
    auto run = [&] {
      auto small = load<pack_t>(a.data());
      auto big = load<pack_t>(b.data());

      REQUIRE(small < big);
      REQUIRE_FALSE(small < small);
      REQUIRE(small <= big);
      REQUIRE(small <= small);

      REQUIRE(big > small);
      REQUIRE(big >= small);
      REQUIRE(small >= small);
    };

    run();

    std::swap(a, b);
    a[0] = small_v;
    b[0] = big_v;
    run();

    a[0] = big_v;
    a[1] = small_v;
    b[1] = big_v;
    run();
  }
}

TEMPLATE_TEST_CASE("simd.pack.comparisons_pairwise", "[simd]", ALL_TEST_PACKS) {
  using pack_t = TestType;
  using vbool = vbool_t<pack_t>;

  using scalar = scalar_t<pack_t>;
  using bool_t = scalar_t<vbool>;

  constexpr size_t size = size_v<pack_t>;

  alignas(pack_t) std::array<scalar, size> a, b;
  alignas(vbool) std::array<bool_t, size> expected, actual;

  const scalar small_v = (scalar)0;
  const scalar big_v = (scalar)(1);

  a.fill(small_v);
  b.fill(big_v);

  const bool_t false_ = 0, true_ = all_ones<bool_t>();

  SECTION("equal_pairwise") {
    auto run = [&] {
      auto x = load<pack_t>(a.data());
      auto y = load<pack_t>(b.data());

      store(actual.data(), equal_pairwise(x, y));
      REQUIRE(expected == actual);
    };

    expected.fill(false_);
    run();

    b = a;
    expected.fill(true_);
    run();

    b[1] = big_v;
    expected[1] = false_;
    run();
  }

  SECTION("greater_pairwise") {
    auto run = [&] {
      auto x = load<pack_t>(a.data());
      auto y = load<pack_t>(b.data());

      store(actual.data(), greater_pairwise(x, y));
      REQUIRE(expected == actual);
    };

    expected.fill(false_);
    run();

    std::swap(a, b);
    expected.fill(true_);
    run();

    a.fill(small_v);
    b.fill(small_v);
    expected.fill(false_);
    run();

    a[1] = big_v;
    expected[1] = true_;
    run();
  }

  SECTION("vbool_tests") {
    vbool mask;
    auto eq = [&]() mutable {
      auto x = load<pack_t>(a.data());
      auto y = load<pack_t>(b.data());

      mask = equal_pairwise(x, y);
    };
    eq();

    REQUIRE_FALSE(all_true(mask));
    REQUIRE_FALSE(any_true(mask));
    REQUIRE_FALSE(first_true(mask));
    REQUIRE_FALSE(any_true_ignore_first_n(mask, 0));
    REQUIRE_FALSE(first_true_ignore_first_n(mask, 0));
    REQUIRE_FALSE(any_true_ignore_first_n(mask, 1));
    REQUIRE_FALSE(first_true_ignore_first_n(mask, 1));

    b = a;
    eq();

    REQUIRE(all_true(mask));
    REQUIRE(any_true(mask));
    REQUIRE(first_true(mask) == 0u);
    REQUIRE(any_true_ignore_first_n(mask, 0));
    REQUIRE(first_true_ignore_first_n(mask, 0) == 0u);
    REQUIRE(any_true_ignore_first_n(mask, 1));
    REQUIRE(first_true_ignore_first_n(mask, 1) == 1u);

    b[0] = big_v;
    eq();

    REQUIRE_FALSE(all_true(mask));
    REQUIRE(any_true(mask));
    REQUIRE(first_true(mask) == 1u);
    REQUIRE(any_true_ignore_first_n(mask, 0));
    REQUIRE(first_true_ignore_first_n(mask, 0) == 1u);
    REQUIRE(any_true_ignore_first_n(mask, 1));
    REQUIRE(first_true_ignore_first_n(mask, 1) == 1u);

    a.fill(small_v);
    b.fill(big_v);
    b[0] = small_v;
    eq();

    REQUIRE_FALSE(all_true(mask));
    REQUIRE(any_true(mask));
    REQUIRE(*first_true(mask) == 0u);
    REQUIRE(any_true_ignore_first_n(mask, 0));
    REQUIRE(first_true_ignore_first_n(mask, 0) == 0u);
    REQUIRE_FALSE(any_true_ignore_first_n(mask, 1));
    REQUIRE_FALSE(first_true_ignore_first_n(mask, 1));
  }
}

TEMPLATE_TEST_CASE("simd.pack.arithmetic", "[simd]", ALL_TEST_PACKS) {
  using pack_t = TestType;
  using scalar = scalar_t<pack_t>;
  constexpr size_t size = size_v<pack_t>;

  alignas(pack_t) std::array<scalar, size> a, b, expected, actual;

  const scalar small_v = (scalar)0;
  const scalar big_v = (scalar)(1);

  a.fill(small_v);
  b.fill(big_v);

  SECTION("add_pairwise/operator+/operator+=") {
    auto run = [&] {
      pack_t x = load<pack_t>(a.data());
      pack_t y = load<pack_t>(b.data());

      pack_t res = add_pairwise(x, y);
      store(actual.data(), res);

      REQUIRE(expected == actual);

      res = x;
      res += y;
      store(actual.data(), res);
      REQUIRE(expected == actual);

      res = x + y;
      store(actual.data(), res);
      REQUIRE(expected == actual);
    };

    expected.fill(big_v);
    run();

    a = b;
    expected.fill((scalar)2);
    run();

    b[1] = (scalar)-1;
    expected[1] = small_v;
    run();
  }

  SECTION("sub_pairwise/operator-/operator-=") {
    auto run = [&] {
      pack_t x = load<pack_t>(a.data());
      pack_t y = load<pack_t>(b.data());

      pack_t res = sub_pairwise(y, x);
      store(actual.data(), res);

      REQUIRE(expected == actual);

      res = y;
      res -= x;
      store(actual.data(), res);
      REQUIRE(expected == actual);

      res = y - x;
      store(actual.data(), res);
      REQUIRE(expected == actual);
    };

    expected.fill(big_v);
    run();

    a = b;
    expected.fill((scalar)0);
    run();

    a[1] = (scalar)-1;
    expected[1] = (scalar)2;
    run();
  }
}

TEMPLATE_TEST_CASE("simd.pack.set", "[simd]", ALL_TEST_PACKS) {
  using pack_t = TestType;
  using scalar = scalar_t<pack_t>;
  constexpr size_t size = size_v<pack_t>;

  alignas(pack_t) std::array<scalar, size> expected, actual;

  SECTION("set_all/set_zero") {
    store(actual.data(), set_all<pack_t>((scalar)1));
    expected.fill((scalar)(1));
    REQUIRE(expected == actual);

    store(actual.data(), set_all<pack_t>((scalar)2));
    expected.fill((scalar)(2));
    REQUIRE(expected == actual);

    store(actual.data(), set_zero<pack_t>());
    expected.fill((scalar)(0));
    REQUIRE(expected == actual);
  }
}

TEST_CASE("simd.pack.address_manipulation", "[simd]") {
  SECTION("end_of_page") {
    auto call = [](std::uintptr_t ptr_bits) {
      int* ptr = reinterpret_cast<int*>(ptr_bits);
      auto res = reinterpret_cast<std::uintptr_t>(end_of_page(ptr));
      const int* cptr = reinterpret_cast<const int*>(ptr_bits);
      REQUIRE(res == reinterpret_cast<std::uintptr_t>(end_of_page(cptr)));
      return res;
    };

    REQUIRE(4096 == call(0));
    REQUIRE(4096 == call(1));
    REQUIRE(8192 == call(4096));
    REQUIRE(8192 == call(5000));

    std::string empty;
    REQUIRE(call((std::uintptr_t)empty.c_str()) >
            (std::uintptr_t)empty.c_str());
  }

  SECTION("previous_aligned_address") {
    auto call = [](auto p, std::uintptr_t ptr_bits) {
      using pack_t = decltype(p);
      int* ptr = reinterpret_cast<int*>(ptr_bits);
      auto res = reinterpret_cast<std::uintptr_t>(
          previous_aligned_address<pack_t>(ptr));
      const int* cptr = reinterpret_cast<const int*>(ptr_bits);
      REQUIRE(res == reinterpret_cast<std::uintptr_t>(
                         previous_aligned_address<pack_t>(cptr)));
      return res;
    };

    REQUIRE(0 == call(pack<char, 16>{}, 0));
    REQUIRE(0 == call(pack<char, 16>{}, 1));
    REQUIRE(0 == call(pack<char, 16>{}, 15));
    REQUIRE(16 == call(pack<char, 16>{}, 16));
    REQUIRE(32 == call(pack<char, 16>{}, 35));

    REQUIRE(0 == call(pack<char, 32>{}, 0));
    REQUIRE(0 == call(pack<char, 32>{}, 1));
    REQUIRE(0 == call(pack<char, 32>{}, 15));
    REQUIRE(0 == call(pack<char, 32>{}, 16));
    REQUIRE(32 == call(pack<char, 32>{}, 35));
  }
}

TEMPLATE_TEST_CASE("simd.pack.blend", "[simd]", ALL_TEST_PACKS) {
  using pack_t = TestType;
  using scalar = scalar_t<pack_t>;
  constexpr size_t size = size_v<pack_t>;

  using vbool = vbool_t<pack_t>;
  using bool_t = scalar_t<vbool>;

  alignas(pack_t) std::array<scalar, size> a, b, expected, actual;
  alignas(vbool) std::array<bool_t, size> mask;

  const scalar small_v = (scalar)0;
  const scalar big_v = (scalar)(1);

  a.fill(small_v);
  b.fill(big_v);
  mask.fill(0);

  SECTION("blend") {
    auto run = [&] {
      pack_t x = load<pack_t>(a.data());
      pack_t y = load<pack_t>(b.data());
      vbool m = load<vbool>(mask.data());

      store(actual.data(), blend(x, y, m));
      REQUIRE(expected == actual);
    };
    expected = a;
    run();

    mask.fill(std::numeric_limits<bool_t>::max());
    expected = b;
    run();

    mask[1] = 0;
    expected[1] = small_v;
    run();
  }

  SECTION("min_pairwise") {
    auto run = [&] {
      pack_t x = load<pack_t>(a.data());
      pack_t y = load<pack_t>(b.data());

      store(actual.data(), min_pairwise(x, y));
      REQUIRE(expected == actual);
    };
    expected = a;
    run();

    std::swap(a, b);
    run();

    a[1] = (scalar)-1;
    if (asif_signed_v<scalar>) {
      expected[1] = (scalar)-1;
    }
    run();
  }

  SECTION("max_pairwise") {
    auto run = [&] {
      pack_t x = load<pack_t>(a.data());
      pack_t y = load<pack_t>(b.data());

      store(actual.data(), max_pairwise(x, y));
      REQUIRE(expected == actual);
    };
    expected = b;
    run();

    std::swap(a, b);
    run();

    b[1] = (scalar)2;
    expected[1] = (scalar)2;
    run();

    a[1] = (scalar)-1;
    if (!asif_signed_v<scalar>) {
      expected[1] = (scalar)-1;
    }
    run();
  }
}

TEMPLATE_TEST_CASE("simd.pack.bit", "[simd]", ALL_TEST_PACKS) {
  using pack_t = TestType;
  using scalar = scalar_t<pack_t>;
  using uscalar = unsigned_equivalent<scalar>;

  const scalar FF = (scalar)all_ones<uscalar>();
  const scalar not_one_scalar = (scalar)(~uscalar(1));

  const pack_t zeroes = set_zero<pack_t>();
  const pack_t ones = set_all<pack_t>((scalar)1);
  const pack_t ffs = set_all<pack_t>(FF);
  const pack_t not_1 = set_all<pack_t>(not_one_scalar);

  SECTION("and_") {
    auto run = [](pack_t x, pack_t y) {
      pack_t res = and_(x, y);
      pack_t option = x & y;
      REQUIRE(res == option);

      option = x;
      option &= y;

      REQUIRE(res == option);
      return res;
    };

    REQUIRE(zeroes == run(zeroes, zeroes));
    REQUIRE(zeroes == run(zeroes, ones));
    REQUIRE(zeroes == run(zeroes, ffs));
    REQUIRE(zeroes == run(ffs, zeroes));
    REQUIRE(ones == run(ones, ones));
    REQUIRE(ones == run(ones, ffs));
    REQUIRE(ones == run(ffs, ones));
  }

  SECTION("or_") {
    auto run = [](pack_t x, pack_t y) {
      pack_t res = or_(x, y);
      pack_t option = x | y;
      REQUIRE(res == option);

      option = x;
      option |= y;

      REQUIRE(res == option);
      return res;
    };

    REQUIRE(zeroes == run(zeroes, zeroes));
    REQUIRE(ones == run(zeroes, ones));
    REQUIRE(ffs == run(zeroes, ffs));
    REQUIRE(ffs == run(ffs, zeroes));
    REQUIRE(ones == run(ones, ones));
    REQUIRE(ffs == run(ones, ffs));
  }

  SECTION("xor_") {
    auto run = [](pack_t x, pack_t y) {
      pack_t res = xor_(x, y);
      pack_t option = x ^ y;
      REQUIRE(res == option);

      option = x;
      option ^= y;

      REQUIRE(res == option);
      return res;
    };

    REQUIRE(zeroes == run(zeroes, zeroes));
    REQUIRE(ones == run(zeroes, ones));
    REQUIRE(ffs == run(zeroes, ffs));
    REQUIRE(ffs == run(ffs, zeroes));
    REQUIRE(zeroes == run(ones, ones));
  }

  SECTION("not_x_and_y") {
    REQUIRE(zeroes == not_x_and_y(zeroes, zeroes));
    REQUIRE(ones == not_x_and_y(zeroes, ones));
    REQUIRE(zeroes == not_x_and_y(ones, zeroes));
    REQUIRE(ffs == not_x_and_y(zeroes, ffs));
    REQUIRE(zeroes == not_x_and_y(ffs, zeroes));
    REQUIRE(zeroes == not_x_and_y(ffs, ffs));
  }

  SECTION("not_") {
    auto run = [](pack_t x) {
      pack_t res = not_(x);
      pack_t option = ~x;
      REQUIRE(res == option);
      return res;
    };

    REQUIRE(ffs == run(zeroes));
    REQUIRE(zeroes == run(ffs));
    REQUIRE(not_1 == run(ones));
    REQUIRE(ones == run(not_1));
  }
}

TEMPLATE_TEST_CASE("simd.pack.compress_mask.first16", "[simd]",
                    (pack<std::uint8_t, 16>), (pack<std::uint8_t, 32>)) {
  using pack_t = TestType;
  using scalar = scalar_t<pack_t>;
  constexpr size_t size = size_v<pack_t>;

  alignas(pack_t) std::array<scalar, size> a, b, actual;

  auto run = [&]() mutable {
    pack_t loaded_a = load<pack_t>(a.data());
    pack_t loaded_b = load<pack_t>(b.data());
    auto eq = equal_pairwise(loaded_a, loaded_b);
    auto compressed =
        compress_mask<register_t<pack_t>>(mm::movemask<std::uint8_t>(eq.reg));

    store(actual.data(), pack_t{compressed.first});

    return compressed.second;
  };

  auto test_for_rest_is_zeros = [&](std::uint8_t popcount) {
    return std::all_of(actual.begin() + popcount, actual.end(),
                      [](auto x) { return x == 0; });
  };

  a.fill(1);
  b.fill(2);

  SECTION("only first 8 has trues") {
    REQUIRE(0 == run());

    b[0] = 1;
    REQUIRE(1 == run());
    REQUIRE(0 == actual[0]);
    REQUIRE(test_for_rest_is_zeros(1));

    b.fill(2);
    b[2] = 1;
    b[3] = 1;
    b[9] = 1;
    REQUIRE(3 == run());
    REQUIRE(2 == actual[0]);
    REQUIRE(3 == actual[1]);
    REQUIRE(9 == actual[2]);
    REQUIRE(test_for_rest_is_zeros(3));

    b.fill(2);
    b[1] = 1;
    REQUIRE(1 == run());
    REQUIRE(1 == actual[0]);
    REQUIRE(test_for_rest_is_zeros(1));

    b.fill(2);
    b[0] = 1;
    b[1] = 1;
    REQUIRE(2 == run());
    REQUIRE(0 == actual[0]);
    REQUIRE(1 == actual[1]);
    REQUIRE(test_for_rest_is_zeros(2));

    b.fill(2);
    b[0] = 1;
    b[4] = 1;
    REQUIRE(2 == run());
    REQUIRE(0 == actual[0]);
    REQUIRE(4 == actual[1]);
    REQUIRE(test_for_rest_is_zeros(2));
  }

  SECTION("mixed first/second 8s have trues") {
    b[8] = 1;
    REQUIRE(1 == run());
    REQUIRE(8 == actual[0]);
    REQUIRE(test_for_rest_is_zeros(1));

    b.fill(2);
    b[0] = 1;
    b[8] = 1;
    REQUIRE(2 == run());
    REQUIRE(0 == actual[0]);
    REQUIRE(8 == actual[1]);
    REQUIRE(test_for_rest_is_zeros(2));

    b.fill(2);
    b[0] = 1;
    b[2] = 1;
    b[9] = 1;
    b[15] = 1;
    REQUIRE(4 == run());
    REQUIRE(0 == actual[0]);
    REQUIRE(2 == actual[1]);
    REQUIRE(9 == actual[2]);
    REQUIRE(15 == actual[3]);
    REQUIRE(test_for_rest_is_zeros(4));

    b.fill(2);
    std::fill(b.begin() + 8, b.begin() + 15, 1);
    REQUIRE(7 == run());
    for (std::uint8_t i = 0; i < 7; ++i) {
      REQUIRE(i + 8 == actual[i]);
    }
    REQUIRE(test_for_rest_is_zeros(7));

    std::fill(b.begin(), b.begin() + 16, 1);
    REQUIRE(16 == run());
    for (std::uint8_t i = 0; i < 16; ++i) {
      REQUIRE(i == actual[i]);
    }
    REQUIRE(test_for_rest_is_zeros(16));
  }

  SECTION("all 4 8s") {
    if (size < 32) return;

    b[16] = 1;
    REQUIRE(1 == run());
    REQUIRE(16 == actual[0]);
    REQUIRE(test_for_rest_is_zeros(1));

    b.fill(2);
    b[24] = 1;
    REQUIRE(1 == run());
    REQUIRE(24 == actual[0]);
    REQUIRE(test_for_rest_is_zeros(1));

    b.fill(2);
    b[1] = 1;
    b[2] = 1;
    b[3] = 1;
    b[4] = 1;
    b[9] = 1;
    b[10] = 1;
    b[11] = 1;
    b[17] = 1;
    b[18] = 1;
    b[25] = 1;
    REQUIRE(10 == run());
    REQUIRE(std::array<scalar, size>{1, 2, 3, 4, 9, 10, 11, 17, 18, 25} == actual);

    b.fill(2);
    b[23] = 1;
    b[24] = 1;
    REQUIRE(2 == run());
    REQUIRE(std::array<scalar, size>{23, 24} == actual);


    b.fill(1);
    REQUIRE(32 == run());
    for (std::uint8_t i = 0; i < 32; ++i) {
      REQUIRE(i == actual[i]);
    }
    REQUIRE(test_for_rest_is_zeros(32));
  }
}

TEST_CASE("simd.pack.understanding_shuffle", "[simd]") {
  using pack_t = pack<std::uint8_t, 16>;
  using scalar = scalar_t<pack_t>;
  constexpr size_t size = size_v<pack_t>;

  alignas(pack_t) std::array<scalar, size> a, b;

  std::iota(a.begin(), a.end(), 0);
  b.fill(0);

  auto run = [&]() mutable {
    pack_t loaded_a = load<pack_t>(a.data());
    pack_t loaded_b = load<pack_t>(b.data());
    pack_t res{_mm_shuffle_epi8(loaded_a.reg, loaded_b.reg)};

    alignas(pack_t) std::array<scalar, size> actual;
    store(actual.data(), res);
    return actual;
  };

  REQUIRE(b == run());

  b[0] = 1;
  REQUIRE(b == run());

  b[0] = 15;
  b[1] = 3;
  REQUIRE(b == run());

  b[15] = 1;
  REQUIRE(b == run());
}

}  // namespace
}  // namespace simd
