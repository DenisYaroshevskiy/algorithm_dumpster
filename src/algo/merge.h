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

#ifndef ALGO_MERGE_H
#define ALGO_MERGE_H

#include <functional>

#include "algo/copy.h"

namespace algo {

template <typename I1, typename I2, typename O, typename R>
// require Mergeable<I1, I2, O, R>
O merge_expensive_cmp(I1 f1, I1 l1, I2 f2, I2 l2, O o, R r) {
  if (f1 == l1) goto copySecond;
  if (f2 == l2) goto copyFirst;

  // clang-format off
  while (true) {
    if (r(*f2, *f1)) {
      *o = *f2; ++o; ++f2; if (f2 == l2) goto copyFirst;
    } else {
      *o = *f1; ++o; ++f1; if (f1 == l1) goto copySecond;
    }
  }
  // clang-format on

copySecond:
  return algo::copy(f2, l2, o);
copyFirst:
  return algo::copy(f1, l1, o);
}

template <typename I1, typename I2, typename O>
O merge_expensive_cmp(I1 f1, I1 l1, I2 f2, I2 l2, O o) {
  return algo::merge_expensive_cmp(f1, l1, f2, l2, o, std::less<>{});
}

template <typename I1, typename I2, typename O, typename R>
// require Mergeable<I1, I2, O, R>
O merge(I1 f1, I1 l1, I2 f2, I2 l2, O o, R r) {
  if (f1 == l1) goto copySecond;
  if (f2 == l2) goto copyFirst;

  // clang-format off
  while(true) {
    if (r(*f2, *f1)) goto takeSecond;
    *o = *f1; ++o; ++f1; if (f1 == l1) goto copySecond;
    goto nextCheck;
  takeSecond:
    *o = *f2; ++o; ++f2;  if (f2 == l2) goto copyFirst;
  nextCheck:
    if (r(*f2, *f1)) goto takeSecond;
    *o = *f1; ++o; ++f1; if (f1 == l1) goto copySecond;
  }
  // clang-format on

copySecond:
  return algo::copy(f2, l2, o);
copyFirst:
  return algo::copy(f1, l1, o);
}

template <typename I1, typename I2, typename O>
O merge(I1 f1, I1 l1, I2 f2, I2 l2, O o) {
  return algo::merge(f1, l1, f2, l2, o, std::less<>{});
}

}  // namespace algo

#endif  // ALGO_MERGE_H
