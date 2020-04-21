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

#include <type_traits>

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

TEST_CASE("find_unguarded, basic", "[simd]") {
  one_range_test([](auto f, auto l) {
    constexpr std::size_t small_pack_size = 16 / sizeof(ValueType<decltype(f)>);
    constexpr std::size_t big_pack_size = small_pack_size * 2;

    one_range_find_unguarded_test<small_pack_size>(f, l);
    one_range_find_unguarded_test<big_pack_size>(f, l);
  });
}

TEMPLATE_TEST_CASE("find_unguarded, types", "[simd]",
                   (std::integral_constant<std::size_t, 16>),
                   (std::integral_constant<std::size_t, 32>)) {
  constexpr std::size_t byte_width = TestType{};

  {
    const char* some_str = "abc";
    auto* end = unsq::find_unguarded<byte_width>(some_str, 0);
    STATIC_REQUIRE(std::is_same_v<decltype(end), const char*>);
    REQUIRE(std::strlen(some_str) == static_cast<std::size_t>(end - some_str));
  }
  {
    const std::vector<const int*> v = {nullptr, nullptr};
    REQUIRE(v.begin() == unsq::find_unguarded<byte_width / 8>(v.begin(), nullptr));
  }
}

}  // namespace
}  // namespace unsq
