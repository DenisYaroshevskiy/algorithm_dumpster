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

#ifndef ALGO_NTH_PERMUTATION_H
#define ALGO_NTH_PERMUTATION_H

#include <cassert>
#include <utility>
#include <vector>

#include "algo/factoriadic_representation.h"
#include "algo/find_nth.h"
#include "algo/positions.h"

namespace algo {

template <typename I, typename O, typename N>
// requires ForwardIterator<I> && OutputIterator<0> && Number<N>
O nth_permutation(I f, I l, O o, N n) {
  if (f == l) {
    assert(!n);
    return o;
  }

  // structured bindings and lambdas.
  auto _binding = lift_as_vector(f, l);
  auto& positions = _binding.positions;
  const auto marker = _binding.marker;

  std::vector<DifferenceType<I>> factoriadic_n(positions.size());
  to_factoriadic_representation(std::move(n), factoriadic_n.rbegin());

  for (DifferenceType<I> pos_n : factoriadic_n) {
    // TODO: we can potentially reduce the number of searches by updating
    //       the left boundary.
    // TODO: Using 'colony' data structure could reduce the cost of the
    //       algorithm: https://youtu.be/wBER1R8YyGY
    auto pos_it = find_nth_if_guarantied(positions.begin(), pos_n,
                                         [&](I x) { return x != marker; });
    *o++ = **pos_it;
    *pos_it = marker;
  }

  return o;
}

}  // namespace algo

#endif  // ALGO_NTH_PERMUTATION_H
