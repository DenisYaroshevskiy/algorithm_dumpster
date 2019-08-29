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

#ifndef TEST_ALGO_STABLE_SORT_GENERIC_TEST_H
#define TEST_ALGO_STABLE_SORT_GENERIC_TEST_H

#include <algorithm>
#include <list>
#include <random>
#include <vector>

#include "test/catch.h"

#include "algo/comparisons.h"
#include "test/algo/stability_test_util.h"

namespace algo {
namespace detail {

struct stable_sort_generic_test_impl {
  template <typename Sorter>
  static void run_test(const std::vector<int>& values, Sorter sorter) {
    const auto vec = make_container_of_stable_unique_iota<std::vector>(values);

    auto expected_vec = copy_container_of_stable_unique(vec);
    std::sort(expected_vec.begin(), expected_vec.end());

    {
      auto actual = copy_container_of_stable_unique(vec);
      sorter(actual.begin(), actual.end(), less_by_first{});
      REQUIRE(expected_vec == actual);
    }

    const auto expected_list =
        cast_container_of_stable_unique<std::list>(expected_vec);

    {
      auto actual = cast_container_of_stable_unique<std::list>(vec);
      sorter(actual.begin(), actual.end(), less_by_first{});
      REQUIRE(expected_list == actual);
    }
  }

  std::mt19937 generator{};

  std::vector<int> random_vector(size_t size) {
    std::vector<int> res(size);
    std::uniform_int_distribution<> dis(0, size * 4);  // 25% duplicates

    std::generate(res.begin(), res.end(),
                  [&]() mutable { return dis(generator); });
    return res;
  }

  template <typename Sorter>
  void test_for_all_permutations(size_t size, Sorter sorter) {
    auto vec = random_vector(size);
    std::sort(vec.begin(), vec.end());

    do {
      run_test(vec, sorter);
    } while (std::next_permutation(vec.begin(), vec.end()));
  }

  template <typename Sorter>
  void special_cases(Sorter sorter) {
    {
      std::vector<int> t(1000, 1);
      run_test(t, sorter);
    }
    {
      std::vector<int> t;
      run_test(t, sorter);
    }
  }

  template <typename Sorter>
  void test_small_permutations(Sorter sorter) {
    for (size_t i = 1; i <= 8; ++i) {
      test_for_all_permutations(i, sorter);
    }
  }

  template <typename Sorter>
  void test_rather_big_ranges(Sorter sorter) {
    for (size_t i = 0; i < 100; ++i) {
      auto t = random_vector(10'000 + i);
      run_test(t, sorter);
    }
  }

  template <typename Sorter>
  void run(Sorter sorter) {
    special_cases(sorter);
    test_small_permutations(sorter);
    test_rather_big_ranges(sorter);
  }
};

}  // namespace detail

template <typename Sorter>
void stable_sort_test(Sorter sorter) {
  detail::stable_sort_generic_test_impl{}.run(sorter);
}

}  // namespace algo

#endif  // TEST_ALGO_STABLE_SORT_GENERIC_TEST_H
