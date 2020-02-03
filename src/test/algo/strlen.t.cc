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

#include "algo/strlen.h"

#include <string>

#include "test/catch.h"

namespace algo {
namespace {

template <std::size_t width>
struct strlen_functor {
  std::size_t operator()(const char* s) const {
    return algo::strlen<width>(s);
  }
};

#define ALL_WIDTH (strlen_functor<16>), (strlen_functor<32>)

TEMPLATE_TEST_CASE("algo.simd.strings.strlen", "[algo][simd]", ALL_WIDTH) {
  TestType selected_strlen;

  REQUIRE(selected_strlen("a") == 1);
  REQUIRE(selected_strlen("") == 0);

  for (std::size_t size = 0; size < 128; ++size) {
    const std::string in(size, 'a');
    for (std::size_t offset = 0; offset <= size; ++offset) {
      REQUIRE(selected_strlen(in.c_str() + offset) == size - offset);
    }
  }

  std::string more_than_a_page(4096 + 1, 'a');
  REQUIRE(more_than_a_page.size() == selected_strlen(more_than_a_page.c_str()));

  more_than_a_page = std::string(3'000'000, 'a');
  REQUIRE(more_than_a_page.size() == selected_strlen(more_than_a_page.c_str()));
}

}  // namespace
}  // namespace algo
