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

#include <random>
#include <set>
#include <vector>

#include <benchmark/benchmark.h>

namespace {

constexpr std::size_t kProblemSize = 1000u;

const std::vector<std::int64_t>& ints_test() {
  static const auto res = []() -> std::vector<std::int64_t> {
    std::mt19937 g;
    std::uniform_int_distribution<std::int64_t> dis(
        1, static_cast<std::int64_t>(kProblemSize) * 10);

    std::set<std::int64_t> unique_sorted_ints;
    while (unique_sorted_ints.size() < kProblemSize)
      unique_sorted_ints.insert(dis(g));

    return {unique_sorted_ints.begin(), unique_sorted_ints.end()};
  }();

  return res;
}

}  // namespace

void do_nothing() {
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");

  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");

  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");

  asm volatile("nop");
  asm volatile("nop");
  asm volatile("nop");
}

void benchmark_lb(benchmark::State& state) {
  do_nothing();

  auto input = ints_test();
  auto looking_for = *(input.begin() + 300);

  for (auto _ : state)
    benchmark::DoNotOptimize(
        std::lower_bound(input.begin(), input.end(), looking_for));
}

BENCHMARK(benchmark_lb);
