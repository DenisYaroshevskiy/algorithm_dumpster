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

#include "bench/algorithm_benchmarks/min_bench.h"

#include "unsq/reduce.h"

namespace {

struct unsq_min_value_v1_128 {
  const char* name() const { return "unsq::v1::min_value<128>"; }

  template <typename I>
  auto operator()(I f, I l) const {
    return *unsq::min_value<16 / sizeof(unsq::ValueType<I>)>(f, l);
  }
};

struct unsq_min_value_v1_256 {
  const char* name() const { return "unsq::v1::min_value<256>"; }

  template <typename I>
  auto operator()(I f, I l) const {
    return *unsq::min_value<32 / sizeof(unsq::ValueType<I>)>(f, l);
  }
};

}  // namespace

int main(int argc, char** argv) {
  bench::bench_main<
      bench::min_bench<unsq_min_value_v1_128, unsq_min_value_v1_256>>(argc,
                                                                      argv);
}
