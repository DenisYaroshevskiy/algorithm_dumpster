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

#include <algorithm>
#include <numeric>
#include <type_traits>

#include "bench/bench.h"
#include "bench_generic/declaration.h"
#include "bench_generic/input_generators.h"

#include "unsq/reduce.h"

namespace {

// Driver ---------------------------------------------------------

template <typename T>
struct min_params {
  std::vector<T> data;
};

struct min_driver {
  template <typename Slide, typename Alg, typename T>
  void operator()(Slide, benchmark::State&, Alg, min_params<T>&) const;
};

template <typename Slide, typename Alg, typename T>
BENCH_DECL_ATTRIBUTES void min_driver::operator()(Slide slide,
                                                  benchmark::State& state,
                                                  Alg alg,
                                                  min_params<T>& params) const {
  bench::noop_slide(slide);

  auto& data = params.data;

  for (auto _ : state) {
    auto v = alg(data.begin(), data.end());
    benchmark::DoNotOptimize(data);
    benchmark::DoNotOptimize(v);
  }
}

// Algorithms -----------------------------------------------------

struct std_min_element {
  const char* name() const { return "std::min_element"; }

  template <typename I>
  auto operator()(I f, I l) const {
    return *std::min_element(f, l);
  }
};

struct std_reduce {
  const char* name() const { return "std_reduce"; }

  template <typename I>
  auto operator()(I f, I l) const {
    return std::reduce(f, l, *f, [](auto x, auto y) { return std::min(x, y); });
  }
};

struct unsq_min_value_128 {
  const char* name() const { return "unsq::min_value<128>"; }

  template <typename I>
  auto operator()(I f, I l) const {
    return *unsq::min_value<16 / sizeof(unsq::ValueType<I>)>(f, l);
  }
};

struct unsq_min_value_256 {
  const char* name() const { return "unsq::min_value<256>"; }

  template <typename I>
  auto operator()(I f, I l) const {
    return *unsq::min_value<32 / sizeof(unsq::ValueType<I>)>(f, l);
  }
};

// Benchmarks ------------------------------------------------------

struct min_benchmark {
  const char* name() const { return "min benchmark"; }

  min_driver driver() const { return min_driver{}; }

  std::vector<std::size_t> sizes() const { return {40, 1000, 10'000}; }

  std::vector<std::size_t> percentage_points() const { return {100}; }

  auto algorithms() const {
    return bench::type_list<std_min_element, std_reduce, unsq_min_value_128,
                            unsq_min_value_256>{};
  }

  bench::type_list<char, short, int> types() const { return {}; }

  template <typename T>
  auto input(struct bench::type_t<T>, std::size_t size,
             std::size_t /*percentage*/) const {
    std::size_t size_in_elements = size / sizeof(T);
    return min_params<T>{bench::random_vector<T>(size_in_elements)};
  }
};

}  // namespace

int main(int argc, char** argv) {
  benchmark::Initialize(&argc, argv);
  if (benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;

  bench::register_benchmark(min_benchmark{});

  benchmark::RunSpecifiedBenchmarks();
}
