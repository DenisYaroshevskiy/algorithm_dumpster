/*
 * Copyright 2019 Denis Yaroshevskiy
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

#ifndef TEST_BINARY_SEARCH_GENERIC_TEST_H
#define TEST_BINARY_SEARCH_GENERIC_TEST_H

#include <algorithm>
#include <array>
#include <list>
#include <numeric>
#include <vector>

#include "algo/container_cast.h"

namespace algo {
namespace detail {

struct binary_search_generic_test_impl {
  template <typename I, typename Test>
  static void run_for_range(I f, I l, Test test) {
    test(l, l, l, 10);

    I selected_f = std::prev(l);
    while (true) {
      for (int i = *selected_f - 1; i != *(std::prev(l)) + 2; ++i)
        for (I h = selected_f; h != l; ++h) test(selected_f, h, l, i);

      if (selected_f == f) break;
      --selected_f;
    }
  }

  template <typename Alg, typename Control>
  static void test_any_binary_search(Alg alg, Control ctrl) {
    std::vector<int> v_data;
    for (int i = 0; i < 5; ++i)
      for (int j = 0; j < i; ++j) v_data.push_back(i);

    auto test = [&](auto f, auto h, auto l, int v) {
      REQUIRE(alg(f, h, l, v) == ctrl(f, l, v));
    };

    run_for_range(v_data.begin(), v_data.end(), test);

    auto l_data = container_cast<std::list>(v_data);
    run_for_range(l_data.begin(), l_data.end(), test);
  }
};

}  // namespace detail

template <typename Alg>
void test_lower_bound(Alg alg) {
  detail::binary_search_generic_test_impl::test_any_binary_search(
      [&](auto f, auto, auto l, const auto& v) { return alg(f, l, v); },
      [](auto f, auto l, const auto& v) { return std::lower_bound(f, l, v); });
}

template <typename Alg>
void test_lower_bound_n(Alg alg) {
  detail::binary_search_generic_test_impl::test_any_binary_search(
      [&](auto f, auto, auto l, const auto& v) {
        return alg(f, std::distance(f, l), v);
      },
      [](auto f, auto l, const auto& v) { return std::lower_bound(f, l, v); });
}

template <typename Alg>
void test_lower_bound_hinted(Alg alg) {
    detail::binary_search_generic_test_impl::test_any_binary_search(
      [&](auto f, auto h, auto l, const auto& v) { return alg(f, h, l, v); },
      [](auto f, auto l, const auto& v) { return std::lower_bound(f, l, v); }
    );
}

}  // namespace algo

#endif  // TEST_BINARY_SEARCH_GENERIC_TEST_H
