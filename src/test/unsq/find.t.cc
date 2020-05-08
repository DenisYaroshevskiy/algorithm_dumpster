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

#include "test/catch.h"
#include "test/unsq/test_input.h"

namespace unsq {
namespace {

template <std::size_t width, typename I>
void one_range_find_test(I f, I l) {
  if (sizeof(ValueType<I>) == 1) {
    l = std::min(l, f + 128);
  }

  std::iota(f, l, ValueType<I>(0));

  REQUIRE(unsq::find<width>(f, l, ValueType<I>{}) ==
          std::find(f, l, ValueType<I>{}));

  for (I cur = f; cur != l; ++cur) {
    REQUIRE(unsq::find<width>(f, l, *cur) == cur);
    REQUIRE(unsq::find_unguarded<width>(f, *cur) == cur);
  }

  if (l - f < 36) return;

  l = f + 35;  // l is writable

  // masks are working
  // before mask
  for (I cur = f + 1; cur != l; ++cur) {
    for (I before = f; before != cur; ++before) {
      REQUIRE(unsq::find<width>(cur, l, *before) - f == l - f);
      auto tmp = *l;
      *l = *before;
      REQUIRE(unsq::find_unguarded<width>(cur, *before) - f == l - f);
      *l = tmp;
    }
  }

  // after mask
  for (I cur = f; cur != f + 35; ++cur) {
    for (I after = cur; after != f + 35; ++after) {
      REQUIRE(unsq::find<width>(f, cur, *after) == cur);
    }
  }
}

TEMPLATE_TEST_CASE("unsq.find/unsq.find_unguarded", "[simd]",
                   (std::integral_constant<std::size_t, 16>),
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
}  // namespace
}  // namespace unsq
