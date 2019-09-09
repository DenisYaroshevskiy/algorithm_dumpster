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

#include "bench_generic/counting_benchmark.h"

namespace bench {

template <size_t total_size>
inline void set_every_5th_percent(counting_benchmark* b) {
  for (int i = 0; i <= 100; i += 5) {
    b->args({static_cast<int>(total_size), i});
  }
}

}  // namespace bench

#endif  // BENCH_SET_PARAMETERS_H
