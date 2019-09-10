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

#include "bench_generic/apply_rearrangment_function_objects.h"
#include "bench_generic/counting_benchmark.h"
#include "bench_generic/input_generators.h"
#include "bench_generic/set_counting_parameters.h"

namespace {

template <typename Alg, typename T>
void apply_rearrangement_counting_bench(const std::vector<int>& args) {
  const size_t size = static_cast<size_t>(args[0]);
  const int percentage = args[1];

  auto raw_data = bench::random_vector<T>(size);

  std::vector<bench::counting_wrapper<T>> data(raw_data.begin(),
                                               raw_data.end());
  auto positions = shuffled_positions(data, size, percentage);
  std::vector<bench::counting_wrapper<T>> opt_output(size);

  Alg{}(positions.begin(), positions.end(), data.begin(), data.end(),
        opt_output.begin());
}

}  // namespace

int main() {
  bench::counting_benchmark b(std::cout);
  bench::set_every_5th_percent<1000>(&b);

  b.run("algo_apply_rearrangment_move",
        apply_rearrangement_counting_bench<bench::algo_apply_rearrangment_move,
                                           int>);
  b.run(
      "algo_apply_rearrangment",
      apply_rearrangement_counting_bench<bench::algo_apply_rearrangment, int>);

  b.run("algo_apply_rearrangment_no_marker",
        apply_rearrangement_counting_bench<
            bench::algo_apply_rearrangment_no_marker, int>);
}
