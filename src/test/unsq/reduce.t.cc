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

#include "unsq/reduce.h"

#include "test/catch.h"
#include "test/unsq/test_input.h"

namespace unsq {
namespace {

template <std::size_t width, typename I>
void one_range_reduce_sum_test(I f, I l) {
  if (sizeof(ValueType<I>) == 1) {
    l = std::min(l, f + 128);
  }

  std::fill(f, l, (ValueType<I>)1);

  // Iteration pattern is tested with find.
  // Masks are tested with min_value/max_value
  ValueType<I> expected = static_cast<ValueType<I>>(l - f);
  REQUIRE(unsq::reduce<width>(f, l) == expected);
}

template <std::size_t width, typename I>
void one_range_min_max_value_test(I f, I l) {
  if (f == l) {
    REQUIRE(!unsq::min_value<width>(f, l));
    REQUIRE(!unsq::max_value<width>(f, l));
    return;
  }

  std::fill(f, l, (ValueType<I>)1);
  auto mid = f + ((l - f) >> 1);
  *mid = (ValueType<I>)0;
  REQUIRE(unsq::min_value<width>(f, l) == *mid);
  *mid = (ValueType<I>)2;
  REQUIRE(unsq::max_value<width>(f, l) == *mid);
  *mid = (ValueType<I>)1;

  if (l - f < 36) return;
  l = f + 35;  // l is writable

  std::iota(f, l, ValueType<I>{});

  // masks are working, using min
  for (I cur = f; cur != l; ++cur) {
    REQUIRE(unsq::min_value<width>(cur, l) == *cur);
  }

  // after mask, using max
  for (I cur = l; cur != f;) {
    --cur;
    REQUIRE(unsq::max_value<width>(f, cur + 1) == *cur);
  }
}

TEMPLATE_TEST_CASE("unsq.reduce", "[unsq][simd][reduce]",
                   (std::integral_constant<std::size_t, 16>),
                   (std::integral_constant<std::size_t, 32>)) {
  constexpr std::size_t byte_width = TestType{};

  one_range_test([](auto f, auto l) {
    static constexpr std::size_t width =
        byte_width / sizeof(ValueType<decltype(f)>);
    one_range_reduce_sum_test<width>(f, l);
    one_range_min_max_value_test<width>(f, l);
  });
}

}  // namespace
}  // namespace unsq
