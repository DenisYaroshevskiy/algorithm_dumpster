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

#ifndef BENCH_GENERIC_LOWER_BOUND_H
#define BENCH_GENERIC_LOWER_BOUND_H

#include <benchmark/benchmark.h>

#include "bench_generic/declaration.h"
#include "bench_generic/input_generators.h"

namespace bench {

template <typename Alg, typename R, typename V, typename Cmp>
BENCH_DECL_ATTRIBUTES void lower_bound_common(benchmark::State& state,
                                              const R& r, const V& v, Cmp cmp) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(Alg{}(r.begin(), r.end(), v, cmp));
  }
}

template <typename Alg, typename T>
void lower_bound_vec(benchmark::State& state) {
  const size_t size = static_cast<size_t>(state.range(0));
  const size_t percentage = static_cast<size_t>(state.range(1));

  const auto input = sorted_vector<T>(size);
  const T value = input[(size - 1) * percentage / 100];

  lower_bound_common<Alg>(state, input, value, std::less<>{});
}

}  // namespace bench

#endif  // BENCH_GENERIC_LOWER_BOUND_H
