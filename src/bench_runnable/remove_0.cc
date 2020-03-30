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

#include "bench_generic/remove.h"

#include <algorithm>

#include "unsq/remove.h"

#include "bench_generic/set_parameters.h"

namespace bench {

struct baseline_remove {
  template <typename I, typename T>
  I operator()(I f, I, const T&) {
    return f;
  }
};

struct std_remove {
  template <typename I, typename T>
  I operator()(I f, I l, const T& v) {
    return std::remove(f, l, v);
  }
};

struct unsq_remove_16 {
  template <typename I, typename T>
  I operator()(I f, I l, const T& v) {
    return unsq::remove<16 / sizeof(unsq::ValueType<I>)>(f, l, v);
  }
};

struct unsq_remove_32 {
  template <typename I, typename T>
  I operator()(I f, I l, const T& v) {
    return unsq::remove<32 / sizeof(unsq::ValueType<I>)>(f, l, v);
  }
};

BENCHMARK_TEMPLATE(remove_0, SELECTED_ALGORITHM, SELECTED_TYPE)
    ->Apply(set_every_5th_percent<SELECTED_NUMBER>);

}  // namespace bench
