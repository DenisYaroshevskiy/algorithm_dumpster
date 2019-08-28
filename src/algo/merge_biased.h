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

#ifndef ALGO_MERGE_BIASED_H
#define ALGO_MERGE_BIASED_H

#include <functional>

#include "algo/binary_search_biased.h"
#include "algo/copy.h"
#include "algo/type_functions.h"

namespace algo {

template <typename I1, typename I2, typename O, typename R>
// require Mergeable<I1, I2, O, R> && ForwardIterator<I1>
O merge_biased_first(I1 f1, I1 l1, I2 f2, I2 l2, O o, R r) {
  if (f1 == l1) goto copySecond;
  if (f2 == l2) goto copyFirst;

  while (true) {
    // clang-format off
    if (r(*f2, *f1)) goto takeSecond;
    *o = *f1; ++o; ++f1; if (f1 == l1) goto copySecond;
    goto nextCheck;
  takeSecond:
    *o = *f2; ++o; ++f2; if (f2 == l2) goto copyFirst;
  nextCheck:
    if (r(*f2, *f1)) goto takeSecond;
    *o = *f1; ++o; ++f1;if (f1 == l1) goto copySecond;
    if (r(*f2, *f1)) goto takeSecond;
    *o = *f1; ++o; ++f1; if (f1 == l1) goto copySecond;
    if (r(*f2, *f1)) goto takeSecond;
    *o = *f1; ++o; ++f1; if (f1 == l1) goto copySecond;
    // clang-format on

    I1 next_f1 = algo::point_closer_to_upper_bound(f1, l1, *f2, r);
    o = algo::copy(f1, next_f1, o);
    f1 = next_f1;
  }

copySecond:
  return algo::copy(f2, l2, o);
copyFirst:
  return algo::copy(f1, l1, o);
}

template <typename I1, typename I2, typename O>
O merge_biased_first(I1 f1, I1 l1, I2 f2, I2 l2, O o) {
  return merge_biased_first(f1, l1, f2, l2, o, std::less<>{});
}

template <typename I1, typename I2, typename O, typename R>
// require Mergeable<I1, I2, O, R> && ForwardIterator<I1>
O merge_biased_second(I1 f1, I1 l1, I2 f2, I2 l2, O o, R r) {
  if (f1 == l1) goto copySecond;
  if (f2 == l2) goto copyFirst;

  while (true) {
    // clang-format off
    if (!r(*f2, *f1)) goto takeFirst;
    *o = *f2; ++o; ++f2; if (f2 == l2) goto copyFirst;
    goto nextCheck;
  takeFirst:
    *o = *f1; ++o; ++f1; if (f1 == l1) goto copySecond;
  nextCheck:
    if (!r(*f2, *f1)) goto takeFirst;
    *o = *f2; ++o; ++f2; if (f2 == l2) goto copyFirst;
    if (!r(*f2, *f1)) goto takeFirst;
    *o = *f2; ++o; ++f2; if (f2 == l2) goto copyFirst;
    if (!r(*f2, *f1)) goto takeFirst;
    *o = *f2; ++o; ++f2; if (f2 == l2) goto copyFirst;
    // clang-format on

    I2 next_f2 = point_closer_to_lower_bound(f2, l2, *f1, r);
    o = algo::copy(f2, next_f2, o);
    f2 = next_f2;
  }

copySecond:
  return algo::copy(f2, l2, o);
copyFirst:
  return algo::copy(f1, l1, o);
}

template <typename I1, typename I2, typename O>
O merge_biased_second(I1 f1, I1 l1, I2 f2, I2 l2, O o) {
  return merge_biased_second(f1, l1, f2, l2, o, std::less<>{});
}

}  // namespace algo

#endif  // ALGO_MERGE_BIASED_H
