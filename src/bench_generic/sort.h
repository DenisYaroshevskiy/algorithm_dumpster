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

#ifndef BENCH_GENERIC_SORT_H
#define BENCH_GENERIC_SORT_H

#include <functional>
#include <vector>

#include <benchmark/benchmark.h>
#include <boost/multiprecision/cpp_int.hpp>

#include "bench_generic/declaration.h"
#include "bench_generic/input_generators.h"

namespace bench {

template <typename Alg, typename R, typename Cmp>
BENCH_DECL_ATTRIBUTES void sort_common(benchmark::State& state, const R& r,
                                       Cmp cmp) {
  for (auto _ : state) {
    R copy = r;
    Alg{}(copy.begin(), copy.end(), cmp);
    benchmark::DoNotOptimize(copy);
  }
}

template <typename Alg, typename T>
void sort_vec(benchmark::State& state) {
  const size_t size = static_cast<size_t>(state.range(0));
  const int percentage = static_cast<int>(state.range(1));

  std::vector<T> vec = shuffled_vector(
      size, percentage, [](size_t size) { return sorted_vector<T>(size); });

  sort_common<Alg>(state, vec, std::less<>{});
}

template <typename Alg, typename T>
void sort_vec_size(benchmark::State& state) {
  const size_t initial_size = static_cast<size_t>(state.range(0));
  const size_t multiplier = static_cast<size_t>(state.range(1));

  auto vec = random_vector<T>(initial_size * multiplier);

  sort_common<Alg>(state, vec, std::less<>{});
}

}  // namespace bench

#endif  // BENCH_GENERIC_SORT_H
