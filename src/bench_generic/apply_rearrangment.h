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

#include <benchmark/benchmark.h>
#include <boost/multiprecision/cpp_int.hpp>

#include "algo/factorial.h"
#include "algo/make_vector_of_iterators.h"
#include "algo/nth_permutation.h"
#include "bench_generic/declaration.h"
#include "bench_generic/input_generators.h"

namespace bench {

template <typename Alg, typename I>
BENCH_DECL_ATTRIBUTES void apply_rearrangment_common(
    benchmark::State& state, const std::vector<I>& saved_positions, I base,
    I marker) {
  for (auto _ : state) {
    std::vector<I> positions = saved_positions;
    Alg{}(positions.begin(), positions.end(), base, marker);
    benchmark::DoNotOptimize(base);
  }
}

template <typename Alg, typename T>
void apply_rearrangment_vec(benchmark::State& state) {
  const size_t size = static_cast<size_t>(state.range(0));
  const int percentage = static_cast<int>(state.range(1));

  auto data = bench::random_vector<T>(size);
  using I = typename std::vector<T>::iterator;
  std::vector<I> positions(size);

  {
    auto initial_positions = algo::make_vector_of_iterators(data.begin(), data.end());
    using big_int = boost::multiprecision::cpp_int;
    const big_int selected_permutation =
        (algo::factorial<big_int>(static_cast<int>(size)) - 1) * percentage /
        100;

    algo::nth_permutation(initial_positions.begin(), initial_positions.end(),
                          positions.begin(), selected_permutation);
  }

  apply_rearrangment_common<Alg>(state, positions, data.begin(), data.end());
}

}  // namespace bench

#endif  // BENCH_GENERIC_APPLY_REARRANGEMENT_H
