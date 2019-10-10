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

#ifndef BENCH_GENERIC_MERGE_H
#define BENCH_GENERIC_MERGE_H

#include <functional>
#include <vector>

#include <benchmark/benchmark.h>

#include "bench_generic/declaration.h"
#include "bench_generic/input_generators.h"

namespace bench {

template <typename Alg, typename RX, typename RY, typename RO, typename Cmp>
BENCH_DECL_ATTRIBUTES void merge_common(benchmark::State& state, RX&& rx,
                                        RY&& ry, RO&& ro, Cmp cmp) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(
        Alg{}(rx.begin(), rx.end(), ry.begin(), ry.end(), ro.begin(), cmp));
  }
}

template <typename Alg, typename T>
void merge_vec(benchmark::State& state) {
  const size_t size = static_cast<size_t>(state.range(0));
  const size_t percentage = static_cast<size_t>(state.range(1));

  const size_t y_size = size * percentage / 100;
  const size_t x_size = size - y_size;

  auto [x_vec, y_vec] = two_sorted_vectors<T>(x_size, y_size);
  std::vector<T> o_vec(x_size + y_size);

  merge_common<Alg>(state, x_vec, y_vec, o_vec, std::less<>{});
}

template <size_t small_size, typename Alg, typename T>
void merge_with_small(benchmark::State& state) {
  const size_t size = static_cast<size_t>(state.range(0));
  const size_t percentage = static_cast<size_t>(state.range(1));

  const size_t y_size = small_size * percentage / 100;
  const size_t x_size = size - y_size;

  auto [x_vec, y_vec] = two_sorted_vectors<T>(x_size, y_size);
  std::vector<T> o_vec(x_size + y_size);

  merge_common<Alg>(state, x_vec, y_vec, o_vec, std::less<>{});
}

}  // namespace bench

#endif  // BENCH_GENERIC_MERGE_H
