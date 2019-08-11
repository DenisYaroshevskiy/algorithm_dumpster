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

#ifndef ALGO_FACTORIAL_H
#define ALGO_FACTORIAL_H

namespace algo {

template <typename ResultN, typename N>
// require Integral<ResultN> && Integral<N>
constexpr ResultN factorial(N n) {
  ResultN res{1};

  for (N i = 2; i <= n; ++i) {
    res *= i;
  }

  return res;
}

}  // namespace algo

#endif  // ALGO_FACTORIAL_H
