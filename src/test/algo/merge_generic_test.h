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

#ifndef TEST_MERGE_GENERIC_TEST_H
#define TEST_MERGE_GENERIC_TEST_H

#include <algorithm>
#include <cstddef>
#include <list>
#include <random>
#include <vector>

#include "test/catch.h"

#include "algo/comparisons.h"
#include "test/algo/stability_test_util.h"

namespace algo {
namespace detail {

struct merge_generic_test_impl {
  static constexpr size_t kTestSize = 100;

  template <typename R, typename Merger>
  static void run_plain_test(const R& xc, const R& yc, Merger merger) {
    std::size_t res_size =
        static_cast<std::size_t>(xc.size() + yc.size());

    using T = typename R::value_type;

    std::vector<T> actual(res_size);
    std::vector<T> expected(res_size);

    std::merge(xc.begin(), xc.end(), yc.begin(), yc.end(), expected.begin());
    merger(xc.begin(), xc.end(), yc.begin(), yc.end(), actual.begin());
    REQUIRE(expected == actual);
  }

  // Using back_inserter to emulate a nasty output iterator.
  template <typename Merger, typename XC, typename YC>
  static void run_merge_test(XC xc, YC yc, Merger merger) {
    auto xc_copy = copy_container_of_stable_unique(xc);
    auto yc_copy = copy_container_of_stable_unique(yc);

    using T = typename XC::value_type;

    std::vector<T> actual_output;
    actual_output.reserve(kTestSize);

    merger(
        std::make_move_iterator(xc.begin()), std::make_move_iterator(xc.end()),
        std::make_move_iterator(yc.begin()), std::make_move_iterator(yc.end()),
        std::back_inserter(actual_output), less_by_first{});

    xc = std::move(xc_copy);
    yc = std::move(yc_copy);

    std::vector<T> expected_output;
    expected_output.reserve(kTestSize);

    std::merge(
        std::make_move_iterator(xc.begin()), std::make_move_iterator(xc.end()),
        std::make_move_iterator(yc.begin()), std::make_move_iterator(yc.end()),
        std::back_inserter(expected_output), less_by_first{});

    REQUIRE(expected_output == actual_output);
  }

  static const std::vector<int>& test_data() {
    static std::vector<int> res = [] {
      std::mt19937 g;
      std::uniform_int_distribution<> dis(1, int(kTestSize) * 100);

      std::vector<int> res(kTestSize);
      std::generate(res.begin(), res.end(), [&]() mutable { return dis(g); });

      return res;
    }();
    return res;
  }

  template <typename Merger>
  static void special_cases(Merger merger) {
    std::vector<int> xc(1, 100);
    std::fill(xc.begin() + 30, xc.end(), 3);
    std::vector<int> yc{3};
    run_plain_test(xc, yc, merger);
  }

  template <typename Merger>
  static void run(Merger merger) {
    special_cases(merger);

    const auto& test_ints = test_data();

    for (std::size_t total_size = 0; total_size <= kTestSize; ++total_size) {
      for (std::size_t x_size = 0; x_size <= total_size; ++x_size) {
        std::vector<int> x_ints{test_ints.begin(), test_ints.begin() + x_size};
        std::vector<int> y_ints{test_ints.begin() + x_size,
                                test_ints.begin() + total_size};

        std::sort(x_ints.begin(), x_ints.end());
        std::sort(y_ints.begin(), y_ints.end());

        run_plain_test(x_ints, y_ints, merger);

        auto vector_test_x =
            make_container_of_stable_unique<std::vector>(x_ints, 0);
        auto vector_test_y =
            make_container_of_stable_unique<std::vector>(y_ints, 1);

        auto list_test_x =
            make_container_of_stable_unique<std::list>(x_ints, 0);
        auto list_test_y =
            make_container_of_stable_unique<std::list>(y_ints, 1);

        // Run all tests.
        run_merge_test(std::move(vector_test_x), std::move(vector_test_y),
                       merger);
        run_merge_test(std::move(list_test_x), std::move(list_test_y), merger);
      }
    }
  }
};

}  // namespace detail

template <typename Merger>
void merge_test(Merger merger) {
  detail::merge_generic_test_impl::run(merger);
}

}  // namespace algo

#endif  // TEST_MERGE_GENERIC_TEST_H
