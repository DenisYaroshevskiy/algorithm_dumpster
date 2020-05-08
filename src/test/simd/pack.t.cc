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
}

TEMPLATE_TEST_CASE("simd.pack.top_bits", "[simd]", ALL_TEST_PACKS) {
  using pack_t = TestType;
  using scalar = scalar_t<pack_t>;
  constexpr size_t size = size_v<pack_t>;
  using uscalar = unsigned_equivalent<scalar>;

  const scalar zero = (scalar)0;
  const scalar FF = (scalar)all_ones<uscalar>();

  alignas(pack_t) std::array<scalar, size> a, b;

  auto run = [&](auto... ignore) {
    pack_t loaded = load<pack_t>(a.data());
    return get_top_bits(loaded, ignore...);
  };

  auto run_b = [&](auto... ignore) {
    pack_t loaded = load<pack_t>(b.data());
    return get_top_bits(loaded, ignore...);
  };

  SECTION("get top bits/ignore") {
    a.fill(zero);
    REQUIRE(0 == run().raw);

    a.fill(FF);
    REQUIRE(set_lower_n_bits(sizeof(pack_t)) == run().raw);

    a.fill(zero);
    a[0] = FF;

    REQUIRE(set_lower_n_bits(sizeof(scalar)) == run().raw);

    REQUIRE(0 == run(ignore_first_n_mask<pack_t>(1)).raw);
  }

  SECTION("ignore first n") {
    a.fill(zero);
    a[0] = FF;

    auto run_and_ignore_first = [&](auto... n) {
      auto option1 = ignore_first_n(run(), n...);
      auto option2 = (ignore_first_n_mask<pack_t>(n...) & run());
      REQUIRE(option1 == option2);
      return option1;
    };

    REQUIRE(set_lower_n_bits(sizeof(scalar)) == run_and_ignore_first().raw);
    REQUIRE(0 == run_and_ignore_first(1).raw);

    a.fill(FF);
    b.fill(FF);

    for (std::size_t i = 0; i < size; ++i) {
      REQUIRE(run_b().raw == run_and_ignore_first(i).raw);
      b[i] = zero;
    }

    REQUIRE(run_b().raw == run_and_ignore_first(size).raw);
  }

  SECTION("ignore last n") {
    a.fill(zero);
    a[size - 1] = FF;

    auto run_and_ignore_last = [&](auto... n) {
      auto option1 = ignore_last_n(run(), n...);
      auto option2 = (ignore_last_n_mask<pack_t>(n...) & run());
      REQUIRE(option1 == option2);
      return option1;
    };

    REQUIRE(0 == run_and_ignore_last(1).raw);

    a.fill(FF);
    b.fill(FF);

    REQUIRE(run_b().raw == run_and_ignore_last().raw);

    for (std::size_t i = size; i > 0; --i) {
      REQUIRE(run_b().raw == run_and_ignore_last(size - i).raw);
      b[i - 1] = zero;
    }

    REQUIRE(run_b().raw == run_and_ignore_last(size).raw);
  }

  SECTION("combine ignore") {
    a.fill(zero);
    REQUIRE(0 == run().raw);

    a[0] = FF;
    a[1] = FF;
    a.back() = FF;

    auto mask = combine_ignore(ignore_first_n_mask<pack_t>(2),
                               ignore_last_n_mask<pack_t>(1));

    REQUIRE(0 == run(mask).raw);
  }

  SECTION("first true/all_true") {
    a.fill(zero);
    REQUIRE(std::nullopt == first_true(run()));

    for (std::uint32_t i = size; i; --i) {
      REQUIRE_FALSE(all_true(run()));
      a[i - 1] = FF;
      REQUIRE(i - 1 == first_true(run()));
    }

    REQUIRE(all_true(run()));
  }

  SECTION("operators") {
    using tb = top_bits<pack_t>;
    const tb x{3}, y{5};

    {
      tb expected{3u & 5u};

      auto tmp = x;
      tmp &= y;
      REQUIRE(tmp == expected);
      REQUIRE((x & y) == expected);
    }

    {
      tb expected{3u | 5u};

      auto tmp = x;
      tmp |= y;
      REQUIRE(tmp == expected);
      REQUIRE((x | y) == expected);
    }

    {
      tb expected{~3u};

      REQUIRE(~x == expected);
    }

    {
      REQUIRE(x == x);
      REQUIRE(x != y);

      REQUIRE_FALSE(x < x);
      REQUIRE(x < y);

      REQUIRE(x <= x);
      REQUIRE(x <= y);

      REQUIRE(y > x);
      REQUIRE_FALSE(y > y);

      REQUIRE(y >= y);
      REQUIRE(y >= x);
    }
  }
}

TEMPLATE_TEST_CASE("simd.pack.spread_top_bits", "[simd]", ALL_TEST_PACKS) {
  using pack_t = TestType;
  using scalar = scalar_t<pack_t>;
  constexpr size_t size = size_v<pack_t>;

  using uscalar = unsigned_equivalent<scalar>;

  const scalar zero = (scalar)0;
  const scalar FF = (scalar)all_ones<uscalar>();

  alignas(pack_t) std::array<scalar, size> in;

  auto run = [&] {
    auto loaded = load<pack_t>(in.data());
    auto top_bits = get_top_bits(loaded);
    REQUIRE(loaded == spread_top_bits(top_bits));
  };

  in.fill(zero);

  SECTION("everything under 18 elements") {
    std::size_t until = std::min(std::size_t{18}, size);
    auto test = [&](auto& self, std::size_t i) mutable {
      if (i == until) {
        run();
        return;
      };
      self(self, i + 1);
      in[i] = FF;
      self(self, i + 1);
    };
    test(test, 0);
  }

  SECTION("every other is set") {
    for (std::size_t i = 0; i < size; i += 2) {
      in[i] = zero;
      in[i + 1] = FF;
    }
    run();

    for (std::size_t i = 0; i < size; i += 2) {
      in[i] = FF;
      in[i + 1] = zero;
    }
    run();
  }

  SECTION("last elements") {
    for (std::size_t i = size - 1; i; --i) {
      in[i] = FF;
      run();
    }
    for (std::size_t i = size - 1; i; --i) {
      in[i] = zero;
      run();
    }
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

TEMPLATE_TEST_CASE("simd.pack.compress_mask_epi8", "[simd]",
                   (pack<std::int8_t, 16>), (pack<std::uint8_t, 16>),
                   (pack<std::int16_t, 8>), (pack<std::uint16_t, 8>),
                   (pack<std::int32_t, 8>), (pack<std::uint32_t, 8>)) {
  using pack_t = TestType;
  using vbool = vbool_t<pack_t>;
  using scalar = scalar_t<pack_t>;
  constexpr std::size_t size = size_v<pack_t>;

  alignas(pack_t) std::array<scalar, size> a;

  alignas(vbool) std::array<scalar, size> element_indexes;
  std::iota(element_indexes.begin(), element_indexes.end(), 0);

  if constexpr (sizeof(scalar) == 2) {
    element_indexes = {0x0100, 0x0302, 0x0504, 0x0706,
                       0x0908, 0x0b0a, 0x0d0c, 0x0f0e};
  }

  auto run_compress = [](std::uint32_t mmask) {
    if constexpr (sizeof(pack_t) == 16) {
      return compress_mask_for_shuffle_epi8<scalar>(mmask);
    } else {
      return compress_mask_for_permutevar8x32<scalar>(mmask);
    }
  };

  a.fill(0);

  auto run = [&] {
    alignas(vbool) std::array<scalar, size> expected;
    expected.fill(0);

    std::uint8_t o = 0;
    for (std::uint8_t i = 0; i < size; ++i) {
      if (a[i] == 0) continue;
      expected[o++] = element_indexes[i];
    }

    const pack_t a_loaded = load<pack_t>(a.data());

    const vbool test = simd::greater_pairwise(a_loaded, set_zero<pack_t>());
    const auto mmask = get_top_bits(test).raw;
    auto res = run_compress(mmask);
    REQUIRE(load<vbool>(expected.data()) == vbool{res.first});
    REQUIRE(res.second == o);
  };

  auto test = [&](auto& self, std::size_t i) mutable {
    if (i == size) {
      run();
      return;
    };
    self(self, i + 1);
    a[i] = 1;
    self(self, i + 1);
  };
  test(test, 0);
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

TEMPLATE_TEST_CASE("simd.pack.compress_store_unsafe", "[simd]",
                   ALL_TEST_PACKS) {
  using pack_t = TestType;
  using vbool = vbool_t<pack_t>;
  using scalar = scalar_t<pack_t>;
  using bool_t = scalar_t<vbool>;

  constexpr size_t size = size_v<pack_t>;

  std::array<scalar, size> actual, expected;

  alignas(pack_t) std::array<scalar, size> input;
  alignas(vbool) std::array<bool_t, size> mask;

  for (std::size_t i = 0; i != input.size(); ++i) {
    input[i] = (scalar)i;
  }

  auto run = [&]() mutable {
    const vbool loaded_mask = load<vbool>(mask.data());
    const auto mmask =
        get_top_bits(greater_pairwise(loaded_mask, set_zero<vbool>()));

    const int non_zero_count = size - std::count(mask.begin(), mask.end(), 0);

    actual.fill(0);
    const pack_t loaded_input = load<pack_t>(input.data());

    auto* res = compress_store_unsafe(actual.data(), loaded_input, mmask);
    REQUIRE(non_zero_count == res - actual.data());

    // No guaranties for elements after.
    std::copy(actual.begin() + non_zero_count, actual.end(),
              expected.begin() + non_zero_count);

    return res;
  };

  SECTION("Up to first 4 even elements") {
    std::size_t even_end = std::min(size, 8ul);

    mask.fill(0);
    expected.fill(0);

    for (std::size_t i = 0; i != even_end; i += 2) {
      mask[i] = 1;
      expected[i / 2] = (scalar)i;
    }

    run();

    REQUIRE(expected == actual);
  }

  SECTION("Max first element") {
    scalar buggy_value =
        (scalar)std::numeric_limits<unsigned_equivalent<scalar>>::max() - 5;
    input[0] = buggy_value;

    expected.fill(0);
    expected[0] = buggy_value;

    mask.fill(0);
    mask[0] = 1;

    run();
    REQUIRE(expected == actual);
  }
}

TEMPLATE_TEST_CASE("simd.pack.compress_store_masked", "[simd]",
                   ALL_TEST_PACKS) {
  using pack_t = TestType;
  using vbool = vbool_t<pack_t>;
  using scalar = scalar_t<pack_t>;
  using bool_t = scalar_t<vbool>;

  constexpr size_t size = size_v<pack_t>;

  alignas(pack_t) std::array<scalar, size> input, expected, actual;
  alignas(vbool) std::array<bool_t, size> mask;

  for (std::size_t i = 0; i != input.size(); ++i) {
    input[i] = (scalar)i;
  }

  auto run = [&](std::size_t offset = 0) mutable {
    const vbool loaded_mask = load<vbool>(mask.data());
    const auto mmask =
        get_top_bits(greater_pairwise(loaded_mask, set_zero<vbool>()));

    const int non_zero_count = size - std::count(mask.begin(), mask.end(), 0);

    actual.fill((scalar)-1);
    const pack_t loaded_input = load<pack_t>(input.data());

    auto* res =
        compress_store_masked(actual.data() + offset, loaded_input, mmask);
    REQUIRE(non_zero_count == res - actual.data() - static_cast<int>(offset));
    return res;
  };

  SECTION("Start from all positions") {
    for (std::size_t i = 0; i != size; ++i) {
      mask.fill(0);

      int remaining = static_cast<int>(size - i);

      std::fill(mask.begin(), mask.begin() + remaining, 1);

      run(i);

      expected.fill((scalar)-1);
      std::copy(input.begin(), input.begin() + remaining, expected.begin() + i);

      REQUIRE(expected == actual);
    }
  }

  SECTION("Up to first 4 even elements") {
    std::size_t even_end = std::min(size, 8ul);

    mask.fill(0);
    expected.fill((scalar)-1);
    auto* output_start = expected.begin() + (size - even_end / 2);
    for (std::size_t i = 0; i != even_end; i += 2) {
      mask[i] = 1;
      output_start[i / 2] = (scalar)i;
    }

    run(size - even_end / 2);
    REQUIRE(expected == actual);
  }

  SECTION("Max first element") {
    scalar buggy_value =
        (scalar)std::numeric_limits<unsigned_equivalent<scalar>>::max() - 5;
    input[0] = buggy_value;

    expected.fill((scalar)-1);
    expected[0] = buggy_value;

    mask.fill(0);
    mask[0] = 1;

    run();
    REQUIRE(expected == actual);
  }
}

TEMPLATE_TEST_CASE("simd.pack.swap_adjacent", "[simd]", ALL_TEST_PACKS) {
  using pack_t = TestType;
  using scalar = scalar_t<pack_t>;
  constexpr size_t size = size_v<pack_t>;

  alignas(pack_t) std::array<scalar, size> input, expected;

  std::iota(input.begin(), input.end(), (scalar)0);
  expected = input;

  auto run = [&](auto constant) {
    pack_t loaded = load<pack_t>(input.data());
    const pack_t actual = swap_adjacent_groups<decltype(constant){}()>(loaded);
    REQUIRE(load<pack_t>(expected.data()) == actual);
  };

  SECTION("1") {
    for (std::size_t i = 0; i < size; i += 2) {
      std::swap(expected[i], expected[i + 1]);
    }
    run(std::integral_constant<std::size_t, 1u>{});
  }

  SECTION("2") {
    if constexpr (size >= 4) {
      for (std::size_t i = 0; i < size; i += 4) {
        auto f = expected.begin() + i;
        std::swap_ranges(f, f + 2, f + 2);
      }
      run(std::integral_constant<std::size_t, 2u>{});
    }
  }

  SECTION("4") {
    if constexpr (size >= 8) {
      for (std::size_t i = 0; i < size; i += 8) {
        auto f = expected.begin() + i;
        std::swap_ranges(f, f + 4, f + 4);
      }
      run(std::integral_constant<std::size_t, 4u>{});
    }
  }

  SECTION("8") {
    if constexpr (size >= 16) {
      for (std::size_t i = 0; i < size; i += 16) {
        auto f = expected.begin() + i;
        std::swap_ranges(f, f + 8, f + 8);
      }
      run(std::integral_constant<std::size_t, 8u>{});
    }
  }
}

TEMPLATE_TEST_CASE("simd.pack.reduce", "[simd]", ALL_TEST_PACKS) {
  using pack_t = TestType;
  using scalar = scalar_t<pack_t>;
  constexpr size_t size = size_v<pack_t>;

  alignas(pack_t) std::array<scalar, size> input, expected;

  auto run = [&] {
    const pack_t x = load<pack_t>(input.data());

    // sum
    if constexpr (!std::is_pointer_v<scalar>) {
      scalar expected_value = std::accumulate(input.begin(), input.end(), 0);
      expected.fill(expected_value);

      auto actual =
          reduce(x, [](const pack_t& x, const pack_t& y) { return x + y; });
      REQUIRE(load<pack_t>(expected.data()) == actual);
    }

    // min
    {
      scalar expected_value = *std::min_element(input.begin(), input.end());
      expected.fill(expected_value);

      auto actual = reduce(x, [](const pack_t& x, const pack_t& y) {
        return min_pairwise(x, y);
      });
      REQUIRE(load<pack_t>(expected.data()) == actual);
    }
  };

  std::iota(input.begin(), input.end(), (scalar)0);
  run();
}

}  // namespace
}  // namespace simd
