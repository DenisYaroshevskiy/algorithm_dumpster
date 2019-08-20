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

#include "bench_generic/sort.h"

#include <algorithm>

#include "bench_generic/set_parameters.h"

namespace bench {

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

BENCHMARK_TEMPLATE(sort_int_vec, SELECTED_ALGORITHM)
    ->Apply(set_every_5th_percent<SELECTED_NUMBER>);

}  // namespace bench
