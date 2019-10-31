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

#ifndef BENCH_SET_PARAMETERS_H
#define BENCH_SET_PARAMETERS_H

#include <benchmark/benchmark.h>
#include "bench_generic/counting_benchmark.h"

namespace bench {

template <size_t total_size>
inline void set_every_5th_percent(benchmark::internal::Benchmark* b) {
  for (int i = 0; i <= 100; i += 5) {
    b->Args({static_cast<int>(total_size), i});
  }
}

template <size_t total_size>
inline void set_every_percent(benchmark::internal::Benchmark* b) {
  for (int i = 0; i <= 100; ++i) {
    b->Args({static_cast<int>(total_size), i});
  }
}

template <size_t total_size>
inline void set_every_2th_percent(benchmark::internal::Benchmark* b) {
  for (int i = 0; i <= 100; i += 2) {
    b->Args({static_cast<int>(total_size), i});
  }
}

template <size_t initial_size, size_t increase>
inline void set_5_size_increases(benchmark::internal::Benchmark* b) {
  int size = static_cast<int>(initial_size);
  int mult = static_cast<int>(increase);
  int pow = 1;
  for (int i = 0; i < 5; ++i) {
    b->Args({size, pow});
    pow *= mult;
  }
}

template <size_t total_size>
inline void set_every_int_size(benchmark::internal::Benchmark* b) {
  b->Args({static_cast<int>(total_size), 8});
  b->Args({static_cast<int>(total_size), 16});
  b->Args({static_cast<int>(total_size), 32});
  b->Args({static_cast<int>(total_size), 64});
}

}  // namespace bench

#endif  // BENCH_SET_PARAMETERS_H
