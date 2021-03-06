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

#ifndef BENCH_GENERIC_APPLY_REARRANGEMENT_H
#define BENCH_GENERIC_APPLY_REARRANGEMENT_H

#include <numeric>

#include <benchmark/benchmark.h>
#include <boost/multiprecision/cpp_int.hpp>

#include "algo/factorial.h"
#include "algo/nth_permutation.h"
#include "algo/positions.h"
#include "bench_generic/declaration.h"
#include "bench_generic/input_generators.h"

namespace bench {

template <typename Alg, typename I>
BENCH_DECL_ATTRIBUTES void apply_rearrangment_common(
    benchmark::State& state, const std::vector<I>& saved_positions, I base,
    I marker, I out_output) {
  for (auto _ : state) {
    std::vector<I> positions = saved_positions;
    Alg{}(positions.begin(), positions.end(), base, marker, out_output);
    benchmark::DoNotOptimize(base);
    benchmark::DoNotOptimize(out_output);
  }
}

template <typename Alg, typename T>
void apply_rearrangment_vec(benchmark::State& state) {
  const size_t size = static_cast<size_t>(state.range(0));
  const int percentage = static_cast<int>(state.range(1));

  auto data = bench::random_vector<T>(size);
  auto positions = shuffled_positions(data, size, percentage);
  std::vector<T> opt_output(size);

  apply_rearrangment_common<Alg>(state, positions, data.begin(), data.end(),
                                 opt_output.begin());
}

}  // namespace bench

#endif  // BENCH_GENERIC_APPLY_REARRANGEMENT_H
