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

#include <tuple>
#include <type_traits>
#include <vector>

#include <benchmark/benchmark.h>

#define BENCH_NOINLINE __attribute__((noinline))
#define BENCH_ALWAYS_INLINE __attribute__((always_inline))

namespace bench {

template <std::size_t n>
using index_c = std::integral_constant<std::size_t, n>;

namespace _bench {

template <typename Op, std::size_t... idx>
constexpr void unroll_impl(Op op, std::index_sequence<idx...>) {
  ((void)op(index_c<idx>{}), ...);
}

template <std::size_t n, typename Op>
constexpr void unroll(Op op) {
  unroll_impl(op, std::make_index_sequence<n>{});
}

}  // namespace _bench

constexpr std::size_t kTestAlignmentLimit = 65;

template <std::size_t n>
BENCH_ALWAYS_INLINE void noop_slide(index_c<n>) {
  _bench::unroll<n>([](auto) { asm volatile("nop"); });
}

template <typename BenchGenerator, typename Driver, typename Algorithm>
void register_benchmark(BenchGenerator generator, Driver driver,
                        Algorithm algorithm) {
  auto percentage_points = generator.percentage_points();

  const std::string common_prefix =
      generator.name() + std::string("/algorihtm:") + algorithm.name() +
      "/size:" + std::to_string(generator.size()) + "/percentage_point:";

  for (auto percentage_point : percentage_points) {
    const std::string prefix =
        common_prefix + std::to_string(percentage_point) + "/slide:";
    auto input = generator.input(generator.size(), percentage_point);

    _bench::unroll<kTestAlignmentLimit>([&](auto slide) {
      const std::string name = prefix + std::to_string(slide);

      benchmark::RegisterBenchmark(
          name.c_str(),
          [slide, driver, algorithm, input](benchmark::State& state) mutable {
            driver(slide, state, algorithm, input);
          });
    });
  }
}

}  // namespace bench
