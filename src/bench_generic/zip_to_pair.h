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

#ifndef BENCH_GENERIC_ZIP_TO_PAIR_H
#define BENCH_GENERIC_ZIP_TO_PAIR_H

#include <utility>

#include <benchmark/benchmark.h>

#include "algo/type_functions.h"
#include "algo/uint_tuple.h"

#include "bench_generic/declaration.h"
#include "bench_generic/input_generators.h"

namespace bench {

template <typename Converter, typename N, typename T>
BENCH_DECL_ATTRIBUTES void zip_to_pair_common(benchmark::State& state,
                                              const std::vector<N>& xs,
                                              const std::vector<N>& ys,
                                              T* out) {
  for (auto _ : state) {
    std::transform(xs.begin(), xs.end(), ys.begin(), out, Converter{});
    benchmark::DoNotOptimize(out);
  }
}

template <size_t size, typename T>
void zip_to_pair_one_size(benchmark::State& state) {
  const size_t range_size = state.range(0);
  auto [xs, ys] =
      bench::two_random_vectors<algo::uint_t<size>>(range_size, range_size);
  std::vector<typename T::template type<size>> out{range_size};
  zip_to_pair_common<T>(state, xs, ys, out.data());
}

template <typename T>
void zip_to_pair_bit_size(benchmark::State& state) {
  const size_t bit_size = static_cast<size_t>(state.range(1));
  switch (bit_size) {
    case 8:
      zip_to_pair_one_size<8, T>(state);
      break;
    case 16:
      zip_to_pair_one_size<16, T>(state);
      break;
    case 32:
      zip_to_pair_one_size<32, T>(state);
      break;
    case 64:
      zip_to_pair_one_size<64, T>(state);
      break;
  }
}

}  // namespace bench

#endif  // BENCH_GENERIC_ZIP_TO_PAIR_H
