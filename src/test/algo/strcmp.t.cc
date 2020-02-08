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

#include <cstring>
#include <string>
#include <random>

#include "test/catch.h"

namespace algo {
namespace {

template <std::size_t width>
struct strcmp_functor {
  std::size_t operator()(const char* sx, const char* sy) const {
    return std::strcmp(sx, sy);
  }
};

#define ALL_WIDTH (strcmp_functor<16>), (strcmp_functor<32>)

TEMPLATE_TEST_CASE("algo.simd.strings.strcmp", "[algo][simd][strcmp]", ALL_WIDTH) {
  TestType selected_strcmp;

  std::mt19937 g;
  std::uniform_int_distribution<char> dis('a', 'z');

  auto make_string = [&](std::size_t size) mutable {
    std::string res(size, 'a');
    std::generate(res.begin(), res.end(), [&]() mutable { return dis(g); });
    return res;
  };

  for (std::size_t x_size = 0; x_size < 128; ++x_size) {
    for (std::size_t y_size = 0; y_size < 128; ++y_size) {
      const std::string x = make_string(x_size);
      const std::string y = make_string(y_size);

      REQUIRE(std::strcmp(x.c_str(), y.c_str()) == selected_strcmp(x.c_str(), y.c_str()));
    }
  }

  // More than a page
  {
      const std::string x = make_string(4097);
      const std::string y = make_string(4099);

      REQUIRE(std::strcmp(x.c_str(), y.c_str()) == selected_strcmp(x.c_str(), y.c_str()));
  }

  // Very big ones
  {
      const std::string x = make_string(2'000'010);
      const std::string y = make_string(2'000'012);

      REQUIRE(std::strcmp(x.c_str(), y.c_str()) == selected_strcmp(x.c_str(), y.c_str()));
  }
}

}  // namespace
}  // namespace algo
