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

#include "unsq/find.h"

#include <numeric>
#include <type_traits>
#include <iostream>

#include "test/catch.h"
#include "test/unsq/test_input.h"

namespace unsq {
namespace {

template <std::size_t width, typename I>
void one_range_find_unguarded_test(I f, I l) {
  std::fill(f, l, 5);

  while (f != l) {
    *--l = 4;
    REQUIRE(l == unsq::find_unguarded<width>(f, 4));
  }
}

template <typename I, typename T>
void log(I f, I l, const T& x) {
  std::cout << '[';
  while (f != l) std::cout << *f++ << ' ';
  std::cout << ']';

  std::cout << " x: " << x << std::endl;
}

template <std::size_t width, typename I>
void one_range_find_test(I f, I l) {
  std::iota(f, l, ValueType<I>(0));

  auto run = [](auto _f, auto _l, const auto& x) {
    log(_f, _l, x);
    auto expected = std::find(_f, _l, x);
    auto actual = unsq::find<width>(_f, _l, x);
    REQUIRE(expected - _f == actual - _f);
  };

  run(f, l, ValueType<I>{});

  for (I cur = f; cur != l; ++cur) {
    run(f, l, *cur);

    // masks are working
    for (I after = cur; after != l; ++after) {
      run(f, cur, *after);
    }
  }
}

TEMPLATE_TEST_CASE("find_unguarded", "[simd]",
                   (std::integral_constant<std::size_t, 16>),
                   (std::integral_constant<std::size_t, 32>)) {
  constexpr std::size_t byte_width = TestType{};

  one_range_test([](auto f, auto l) {
    static constexpr std::size_t width =
        byte_width / sizeof(ValueType<decltype(f)>);
    one_range_find_unguarded_test<width>(f, l);
  });

  {
    const char* some_str = "abc";
    auto* end = unsq::find_unguarded<byte_width>(some_str, 0);
    STATIC_REQUIRE(std::is_same_v<decltype(end), const char*>);
    REQUIRE(std::strlen(some_str) == static_cast<std::size_t>(end - some_str));
  }
  {
    const std::vector<const int*> v = {nullptr, nullptr};
    REQUIRE(v.begin() ==
            unsq::find_unguarded<byte_width / 8>(v.begin(), nullptr));
  }
}

TEMPLATE_TEST_CASE("find", "[simd]", (std::integral_constant<std::size_t, 16>),
                   (std::integral_constant<std::size_t, 32>)) {
  constexpr std::size_t byte_width = TestType{};

  one_range_test([](auto f, auto l) {
    static constexpr std::size_t width =
        byte_width / sizeof(ValueType<decltype(f)>);
    one_range_find_test<width>(f, l);
  });

  {
    const char* some_str = "abc";
    auto* end =
        unsq::find<byte_width>(some_str, some_str + std::strlen(some_str), 0);
    STATIC_REQUIRE(std::is_same_v<decltype(end), const char*>);
    REQUIRE(std::strlen(some_str) == static_cast<std::size_t>(end - some_str));
  }
  {
    const std::vector<const int*> v = {nullptr, nullptr};
    REQUIRE(v.begin() ==
            unsq::find<byte_width / 8>(v.begin(), v.end(), nullptr));
  }
}
}  // namespace
}  // namespace unsq
