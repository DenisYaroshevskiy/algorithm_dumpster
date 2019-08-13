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

#ifndef BENCH_GENERIC_COPY_H
#define BENCH_GENERIC_COPY_H

#include <iterator>
#include <vector>

#include <benchmark/benchmark.h>

#include "bench_generic/declaration.h"
#include "bench_generic/input_generators.h"

namespace bench {

template <typename Alg, typename InR, typename OutR>
BENCH_DECL_ATTRIBUTES void copy_revere_iterators_common(benchmark::State& state,
                                                        InR& in, // Non const, because this bug: https://bugs.llvm.org/show_bug.cgi?id=40575
                                                        OutR& out) {
  for (auto _ : state) {
    benchmark::DoNotOptimize(Alg{}(in.rbegin(), in.rend(), out.rbegin()));
  }
}

template <size_t vec_size, typename Alg>
void copy_reverse_iterators_int_vec(benchmark::State& state) {
  auto in_vec = bench::random_vector<int>(vec_size);
  std::vector<int> out_vec(vec_size);

  copy_revere_iterators_common<Alg>(state, in_vec, out_vec);
}

}  // namespace bench

#endif  // BENCH_GENERIC_COPY_H
