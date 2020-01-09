/*
 * Copyright 2020 Denis Yaroshevskiy
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

#include "algo/unroll.h"
#include "bench_generic/input_generators.h"
#include "bench_generic/merge.h"

#include <benchmark/benchmark.h>

#ifdef NOOP_COUNT
static constexpr size_t noop_count = NOOP_COUNT;
#else
static constexpr size_t noop_count = 0;
#endif  // NOOP_COUNT

// clang-format off
template <typename RX, typename RY, typename RO, typename Cmp>
BENCH_DECL_ATTRIBUTES  // Just a noinline
void do_benchmark(benchmark::State& state, RX&& rx, RY&& ry, RO&& ro, Cmp cmp) {
  // Modify alignment
  algo::unroll<noop_count>([] { asm volatile("nop"); });

  for (auto _ : state) {
    benchmark::DoNotOptimize(std::merge(rx.begin(), rx.end(), ry.begin(),
                                        ry.end(), ro.begin(), cmp));
  }
}
// clang-format on

void test_benchmark(benchmark::State& state) {
  static constexpr size_t x_size = 1300;
  static constexpr size_t y_size = 700;

  auto [x_vec, y_vec] = bench::two_sorted_vectors<int>(x_size, y_size);
  std::vector<int> o_vec(x_size + y_size);

  do_benchmark(state, x_vec, y_vec, o_vec, std::less<>{});
}

BENCHMARK(test_benchmark);
