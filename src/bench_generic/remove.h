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

#ifndef BENCH_GENERIC_REMOVE_H_
#define BENCH_GENERIC_REMOVE_H_

#include <vector>

#include <benchmark/benchmark.h>

#include "bench_generic/declaration.h"
#include "bench_generic/input_generators.h"

namespace bench {

template <typename Alg, typename R>
BENCH_DECL_ATTRIBUTES void remove_0_common(benchmark::State& state, R& r, R& r_copy) {
  for (auto _ : state) {
    std::copy(r.begin(), r.end(), r_copy.begin());
    Alg{}(r_copy.begin(), r_copy.end(), 0);
    benchmark::DoNotOptimize(r_copy);
  }
}

template <typename Alg, typename T>
void remove_0(benchmark::State& state) {
  const size_t size = static_cast<size_t>(state.range(0));
  const int percentage = static_cast<int>(state.range(1));

  auto data = vector_with_zeroes<T>(size, percentage);
  auto buffer = data;
  remove_0_common<Alg>(state, data, buffer);
}

}  // namespace bench

#endif  // BENCH_GENERIC_REMOVE_H_
