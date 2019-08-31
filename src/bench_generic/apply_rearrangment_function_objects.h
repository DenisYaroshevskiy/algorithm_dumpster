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

#ifndef BENCH_GENERIC_FUNCTION_OBJECTS_H
#define BENCH_GENERIC_FUNCTION_OBJECTS_H

#include "algo/apply_rearrangment.h"
#include "algo/move.h"
#include "algo/type_functions.h"

#include <vector>

namespace bench {

struct apply_rearrangment_move_and_back {
  template <typename II, typename I>
  void operator()(II f, II l, I base, I) const {
    std::vector<algo::ValueType<I>> tmp(static_cast<size_t>(l - f));
    algo::apply_rearrangment_move(f, l, tmp.begin());
    algo::move(tmp.begin(), tmp.end(), base);
  }
};

struct apply_rearrangment {
  template <typename II, typename I>
  void operator()(II f, II l, I base, I marker) const {
    algo::apply_rearrangment(f, l, base, marker);
  }
};

}  // namespace bench

#endif  // BENCH_GENERIC_FUNCTION_OBJECTS_H
