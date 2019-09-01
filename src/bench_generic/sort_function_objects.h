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

#ifndef BENCH_GENERIC_SORT_FUNCTION_OBJECTS_H
#define BENCH_GENERIC_SORT_FUNCTION_OBJECTS_H

#include "algo/stable_sort.h"

namespace bench {

struct algo_stable_sort_sufficient_allocation {
  template <typename... Args>
  auto operator()(Args&&... args) const {
    return algo::stable_sort_sufficient_allocation(std::forward<Args>(args)...);
  }
};

struct algo_stable_sort_sufficient_allocation_std_merge {
  template <typename... Args>
  auto operator()(Args&&... args) const {
    return algo::stable_sort_sufficient_allocation_std_merge(
        std::forward<Args>(args)...);
  }
};

struct algo_stable_sort_lifting {
  template <typename... Args>
  auto operator()(Args&&... args) const {
    return algo::stable_sort_lifting(std::forward<Args>(args)...);
  }
};

struct std_sort {
  template <typename I, typename Cmp>
  void operator()(I f, I l, Cmp cmp) {
    std::sort(f, l, cmp);
  }
};

struct std_stable_sort {
  template <typename I, typename Cmp>
  void operator()(I f, I l, Cmp cmp) {
    std::stable_sort(f, l, cmp);
  }
};

}  // namespace bench

#endif  // BENCH_GENERIC_SORT_FUNCTION_OBJECTS_H
