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

#include "bench_generic/counting_benchmark.h"
#include "bench_generic/input_generators.h"
#include "bench_generic/set_counting_parameters.h"
#include "bench_generic/sort_function_objects.h"

namespace {

template <typename Alg, typename T>
void sort_permutation_counting_bench(const std::vector<int>& args) {
  const size_t size = static_cast<size_t>(args[0]);
  const int percentage = args[1];

  std::vector<T> raw_vec = bench::shuffled_vector(
      size, percentage,
      [](size_t size) { return bench::sorted_vector<T>(size); });
  std::vector<bench::counting_wrapper<T>> vec(raw_vec.begin(), raw_vec.end());

  Alg{}(vec.begin(), vec.end(), std::less<>{});
}

}  // namespace

int main() {
  bench::counting_benchmark b(std::cout);
  bench::set_every_5th_percent<1000>(&b);

#define ADD_BENCH(name) b.run(#name, sort_permutation_counting_bench<bench::name, int>)

  ADD_BENCH(algo_stable_sort_sufficient_allocation);
  ADD_BENCH(algo_stable_sort_sufficient_allocation_std_merge);
  ADD_BENCH(algo_stable_sort_lifting);
  ADD_BENCH(baseline_sort);
  ADD_BENCH(std_sort);
  ADD_BENCH(std_stable_sort);

#undef ADD_BENCH
}
