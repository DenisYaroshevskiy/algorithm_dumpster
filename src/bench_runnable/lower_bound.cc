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

#include "bench_generic/lower_bound.h"

#include <algorithm>
#include <utility>

#include "algo/binary_search.h"
#include "bench_generic/set_parameters.h"

namespace bench {

struct std_lower_bound {
  template <typename... Args>
  auto operator()(Args&&... args) const {
    return std::lower_bound(std::forward<Args>(args)...);
  }
};

struct algo_lower_bound {
  template <typename... Args>
  auto operator()(Args&&... args) const {
    return algo::lower_bound(std::forward<Args>(args)...);
  }
};

BENCHMARK_TEMPLATE(lower_bound_vec, SELECTED_ALGORITHM, SELECTED_TYPE)
    ->Apply(set_every_5th_percent<SELECTED_NUMBER>);

}  // namespace bench