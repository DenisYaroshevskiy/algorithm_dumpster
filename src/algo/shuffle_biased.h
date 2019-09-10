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

#ifndef ALGO_SHUFFLE_BIASED_H
#define ALGO_SHUFFLE_BIASED_H

#include <random>

#include "algo/type_functions.h"
#include "algo/half_positive.h"

namespace algo {

template <typename I, typename G>
// require RandomAccessIterator<I> && UniformRandomGenerator<G>
void shuffle_biased(I f, I l, DifferenceType<I> limit, G&& g) {
  DifferenceType<I> half_limit = limit - algo::half_positive(limit);
  while (l - f > half_limit) {
    std::shuffle(f, f + std::min(l - f, limit), g);
    f += half_limit;
  }
}

}  // namespace algo

#endif  // ALGO_SHUFFLE_BIASED_H
