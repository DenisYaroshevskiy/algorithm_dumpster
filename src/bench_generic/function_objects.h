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

#ifndef BENCH_GENERIC_FUNCTION_OBJECTS_H
#define BENCH_GENERIC_FUNCTION_OBJECTS_H

#include "algo/binary_search.h"
#include "algo/binary_search_biased.h"
#include "algo/merge.h"
#include "algo/merge_biased.h"

namespace bench {

struct algo_lower_bound {
  template <typename... Args>
  auto operator()(Args&&... args) const {
    return algo::lower_bound(std::forward<Args>(args)...);
  }
};

struct algo_lower_bound_biased {
  template <typename... Args>
  auto operator()(Args&&... args) const {
    return algo::lower_bound_biased(std::forward<Args>(args)...);
  }
};

struct algo_lower_bound_biased_expensive_cmp {
  template <typename... Args>
  auto operator()(Args&&... args) const {
    return algo::lower_bound_biased_expensive_cmp(std::forward<Args>(args)...);
  }
};

struct algo_lower_bound_linear {
  template <typename... Args>
  auto operator()(Args&&... args) const {
    return algo::lower_bound_linear(std::forward<Args>(args)...);
  }
};

struct algo_merge {
  template <typename... Args>
  auto operator()(Args&&... args) const {
    return algo::merge(std::forward<Args>(args)...);
  }
};

struct algo_merge_expensive_cmp {
  template <typename... Args>
  auto operator()(Args&&... args) const {
    return algo::merge_expensive_cmp(std::forward<Args>(args)...);
  }
};

struct algo_merge_biased_first {
  template <typename... Args>
  auto operator()(Args&&... args) const {
    return algo::merge_biased_first(std::forward<Args>(args)...);
  }
};

struct std_lower_bound {
  template <typename... Args>
  auto operator()(Args&&... args) const {
    return std::lower_bound(std::forward<Args>(args)...);
  }
};

struct std_merge {
  template <typename... Args>
  auto operator()(Args&&... args) {
    return std::merge(std::forward<Args>(args)...);
  }
};

}  // namespace bench

#endif  // BENCH_GENERIC_FUNCTION_OBJECTS_H
