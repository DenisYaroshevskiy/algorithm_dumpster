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

template <typename Alg>
void merge_int_vec(benchmark::State& state) {
  const size_t x_size = static_cast<size_t>(state.range(0));
  const size_t y_size = static_cast<size_t>(state.range(1));

  auto [x_vec, y_vec] = two_sorted_vectors<int>(x_size, y_size);
  std::vector<int> o_vec(x_size + y_size);

  merge_common<Alg>(state, x_vec, y_vec, o_vec, std::less<>{});
}

}  // namespace bench

#endif  // BENCH_GENERIC_MERGE_H