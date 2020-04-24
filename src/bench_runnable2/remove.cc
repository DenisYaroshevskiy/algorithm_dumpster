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

#include "unsq/remove.h"

namespace {

// Driver ---------------------------------------------------------

template <typename T>
struct remove_params {
  std::vector<T> data;
  std::vector<T> buffer;  // buffer.size() == data.size()
  T x;
};

struct remove_driver {
  template <typename Slide, typename Alg, typename T>
  void operator()(Slide, benchmark::State&, Alg, remove_params<T>&) const;
};

template <typename Slide, typename Alg, typename T>
BENCH_DECL_ATTRIBUTES void remove_driver::operator()(
    Slide slide, benchmark::State& state, Alg alg,
    remove_params<T>& params) const {
  bench::noop_slide(slide);

  auto& [data, buffer, x] = params;

  for (auto _ : state) {
    std::copy(data.begin(), data.end(), buffer.begin());
    alg(buffer.begin(), buffer.end(), x);
    benchmark::DoNotOptimize(buffer);
  }
}

// Algorithms -----------------------------------------------------

struct std_remove {
  const char* name() const { return "std::remove"; }

  template <typename I, typename T>
  I operator()(I f, I l, const T& v) const {
    return std::remove(f, l, v);
  }
};

struct unsq_remove_128 {
  const char* name() const { return "unsq::remove<16>"; }

  template <typename I, typename T>
  I operator()(I f, I l, const T& v) const {
    return unsq::remove<16 / sizeof(unsq::ValueType<I>)>(f, l, v);
  }
};

struct unsq_remove_256 {
  const char* name() const { return "unsq::remove<32>"; }

  template <typename I, typename T>
  I operator()(I f, I l, const T& v) const {
    return unsq::remove<32 / sizeof(unsq::ValueType<I>)>(f, l, v);
  }
};

// Benchmarks ------------------------------------------------------

struct remove_0s {
  const char* name() const { return "remove 0s"; }

  remove_driver driver() const { return remove_driver{}; }

  std::vector<std::size_t> sizes() const { return {1000}; }

  std::vector<std::size_t> percentage_points() const {
    return {0, 5, 20, 50, 80, 95, 100};
  }

  bench::type_list<std_remove, unsq_remove_128, unsq_remove_256> algorithms()
      const {
    return {};
  }

  bench::type_list<char, short, int> types() const { return {}; }

  template <typename T>
  auto input(struct bench::type_t<T>, std::size_t size, std::size_t percentage) const {
    std::size_t size_in_elements = size / sizeof(T);
    return remove_params<T>{
        bench::vector_with_zeroes<T>(size_in_elements, percentage),
        std::vector<T>(size_in_elements), 0};
  }
};

}  // namespace

int main(int argc, char** argv) {
  benchmark::Initialize(&argc, argv);
  if (benchmark::ReportUnrecognizedArguments(argc, argv)) return 1;

  bench::register_benchmark(remove_0s{});

  benchmark::RunSpecifiedBenchmarks();
}
