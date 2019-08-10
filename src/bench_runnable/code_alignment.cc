#include "algo/unroll.h"
#include "bench_generic/input_generators.h"
#include "bench_generic/merge.h"

#include <benchmark/benchmark.h>

// http://quick-bench.com/GJ2brewZxr0auF9CntT2wE-TLO4

#ifdef NOOP_COUNT
static constexpr size_t noop_count = NOOP_COUNT;
#else
static constexpr size_t noop_count = 0;
#endif  // NOOP_COUNT

void do_nothing() {
  algo::unroll<noop_count>([] { asm volatile("nop"); });
}

struct alg {
  template <typename I, typename Cmp>
  I operator()(I f1, I l1, I f2, I l2, I o, Cmp cmp) {
    return std::merge(f1, l1, f2, l2, o, cmp);
  }
};

void test_benchmark(benchmark::State& state) {
  static constexpr size_t x_size = 1300;
  static constexpr size_t y_size = 700;

  auto [x_vec, y_vec] = bench::two_sorted_vectors<int>(x_size, y_size);
  std::vector<int> o_vec(x_size + y_size);

  for (auto _ : state) {
    benchmark::DoNotOptimize(std::merge(
        x_vec.begin(), x_vec.end(), y_vec.begin(), y_vec.end(), o_vec.begin()));
  }
}

#ifndef USE_ARG
BENCHMARK(test_benchmark);
#else
BENCHMARK(test_benchmark)->Arg(noop_count);
#endif
