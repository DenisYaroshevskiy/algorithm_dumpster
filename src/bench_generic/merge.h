#ifndef BENCH_GENERIC_MERGE_H
#define BENCH_GENERIC_MERGE_H

#include <vector>

#include "benchmark/benchmark.h"

#include "bench_generic/declaration.h"

namespace bench {

template <typename Alg, typename R1, typename R2, typename RO, typename Cmp>
BENCH_DECL_ATTRIBUTES void merge_common(benchmark::State& state, Alg alg,
                                        R1&& r1, R2&& r2, RO&& ro, Cmp cmp) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(
        alg(r1.begin(), r1.end(), r2.begin(), r2.end(), ro.begin(), cmp));
  }
}

}  // namespace bench

#endif  // BENCH_GENERIC_MERGE_H