#ifndef BENCH_GENERIC_SORT_H
#define BENCH_GENERIC_SORT_H

#include <functional>
#include <vector>

#include <iostream>

#include <benchmark/benchmark.h>
#include <boost/multiprecision/cpp_int.hpp>

#include "algorithm/factorial.h"
#include "algorithm/nth_permutation.h"
#include "bench_generic/declaration.h"
#include "bench_generic/input_generators.h"

namespace bench {

template <typename Alg, typename R, typename Cmp>
BENCH_DECL_ATTRIBUTES void sort_common(benchmark::State& state, const R& r,
                                       Cmp cmp) {
  for (auto _ : state) {
    R copy = r;
    Alg{}(copy.begin(), copy.end(), cmp);
    benchmark::DoNotOptimize(copy);
  }
}

template <size_t size, typename Alg>
void sort_int_vec(benchmark::State& state) {
  const int percentage = static_cast<int>(state.range(0));

  auto sorted_vec = sorted_vector<int>(size);

  using big_int = boost::multiprecision::cpp_int;
  const big_int selected_permutation =
      (algo::factorial<big_int>(static_cast<int>(size)) - 1) * percentage / 100;

  std::vector<int> vec(size);
  algo::nth_permutation(sorted_vec.begin(), sorted_vec.end(), vec.begin(),
                        selected_permutation);

  sort_common<Alg>(state, vec, std::less<>{});
}

}  // namespace bench

#endif  // BENCH_GENERIC_SORT_H