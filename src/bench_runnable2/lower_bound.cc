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
#include <string>
#include <type_traits>

#include "bench/bench.h"
#include "bench_generic/declaration.h"
#include "bench_generic/input_generators.h"

#include "algo/binary_search_biased.h"

namespace {

// Driver ---------------------------------------------------------

template <typename T>
struct lower_bound_params {
  std::vector<T> data;  // Should be sorted
  T x;
};

struct lower_bound_driver {
  template <typename Slide, typename Alg, typename T>
  void operator()(Slide, benchmark::State&, Alg, lower_bound_params<T>&) const;
};

template <typename Slide, typename Alg, typename T>
BENCH_DECL_ATTRIBUTES void lower_bound_driver::operator()(
    Slide slide, benchmark::State& state, Alg alg,
    lower_bound_params<T>& params) const {
  bench::noop_slide(slide);

  auto& [data, x] = params;

  for (auto _ : state) {
    benchmark::DoNotOptimize(alg(data.begin(), data.end(), x));
  }
}

// Algorithms -----------------------------------------------------

struct algo_lower_bound_linear {
  const char* name() const { return "algo::lower_bound_linear"; }

  template <typename I, typename T>
  I operator()(I f, I l, const T& v) const {
    return algo::lower_bound_linear(f, l, v);
  }
};

struct algo_lower_bound_biased {
  const char* name() const { return "algo::lower_bound_biased"; }

  template <typename I, typename T>
  I operator()(I f, I l, const T& v) const {
    return algo::lower_bound_biased(f, l, v);
  }
};

struct std_lower_bound {
  const char* name() const { return "std::lower_bound"; }

  template <typename I, typename T>
  I operator()(I f, I l, const T& v) const {
    return std::lower_bound(f, l, v);
  }
};

// Benchmarks ------------------------------------------------------

struct find_lower_bound {
  const char* name() const { return "find lower bound"; }

  lower_bound_driver driver() const { return {}; }

  std::vector<std::size_t> sizes() const { return {4000}; }

  std::vector<std::size_t> percentage_points() const {
    return {5, 20, 50, 80, 95};
  }

  bench::type_list<algo_lower_bound_linear, algo_lower_bound_biased,
                   std_lower_bound>
  algorithms() const {
    return {};
  }

  bench::type_list<int, float> types() const { return {}; }

  template <typename T>
  auto input(struct bench::type_t<T>, std::size_t size,
             std::size_t percentage) const {
    std::size_t size_in_elements = size / sizeof(T);

    auto input = bench::sorted_vector<T>(size_in_elements);
    T value = input[(size_in_elements - 1) * percentage / 100];

    return lower_bound_params<T>{input, value};
  }
};

}  // namespace

int main(int argc, char** argv) {
  benchmark::Initialize(&argc, argv);
  if (benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;

  bench::register_benchmark(find_lower_bound{});

  benchmark::RunSpecifiedBenchmarks();
}
