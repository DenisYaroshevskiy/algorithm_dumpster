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

#include "algo/strcmp.h"

#include <cstring>
#include <string>
#include <random>

#include "test/catch.h"

namespace algo {
namespace {

template <std::size_t width>
struct strcmp_v1_functor {
  int operator()(const char* sx, const char* sy) const {
    int res = algo::v1::strcmp<width>(sx, sy);
    if (res < 0) return -1;
    if (res > 0) return 1;
    return 0;
  }
};

template <std::size_t width>
struct strcmp_functor {
  int operator()(const char* sx, const char* sy) const {
    int res = algo::strcmp<width>(sx, sy);
    if (res < 0) return -1;
    if (res > 0) return 1;
    return 0;
  }
};

#define ALL_CASES (strcmp_v1_functor<16>), (strcmp_v1_functor<32>), \
                  (strcmp_functor<16>), (strcmp_functor<32>)

TEMPLATE_TEST_CASE("algo.simd.strings.strcmp", "[algo][simd][strcmp]", ALL_CASES) {
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
      const std::string z = x + 'a';

      REQUIRE(std::strcmp(x.c_str(), y.c_str()) == selected_strcmp(x.c_str(), y.c_str()));
      REQUIRE(std::strcmp(x.c_str(), z.c_str()) == selected_strcmp(x.c_str(), z.c_str()));
    }
  }

  // More than a page
  {
      const std::string x = make_string(4097);
      const std::string y = make_string(4099);
      const std::string z = x + 'a';


      REQUIRE(std::strcmp(x.c_str(), y.c_str()) == selected_strcmp(x.c_str(), y.c_str()));
      REQUIRE(std::strcmp(x.c_str(), z.c_str()) == selected_strcmp(x.c_str(), z.c_str()));
  }

  // Very big ones
  {
      const std::string x = make_string(2'000'010);
      const std::string y = make_string(2'000'012);
      const std::string z = x + 'a';

      REQUIRE(std::strcmp(x.c_str(), y.c_str()) == selected_strcmp(x.c_str(), y.c_str()));
      REQUIRE(std::strcmp(x.c_str(), z.c_str()) == selected_strcmp(x.c_str(), z.c_str()));
  }
}

}  // namespace
}  // namespace algo
