#ifndef BENCH_SET_PARAMETERS_H
#define BENCH_SET_PARAMETERS_H

#include <benchmark/benchmark.h>

namespace bench {

inline void set_every_5th_percent(benchmark::internal::Benchmark* bench) {
  for (int i = 0; i <= 100; i += 5) {
    bench->Arg(i);
  }
}

}  // namespace bench

#endif  // BENCH_SET_PARAMETERS_H
