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

#ifndef ALGO_MEMOIZED_FUNCTION_H
#define ALGO_MEMOIZED_FUNCTION_H

#include <map>
#include <type_traits>

namespace algo {

template <typename T, typename Op>
// require TotallyOrdered<T> && UnaryFunction<Op, T>
auto memoized_function(Op op) {
  using result_type = decltype(op(std::declval<T>()));
  return [cache = std::map<T, result_type>{}, op](const T& x) mutable {
    auto lb = cache.lower_bound(x);
    if (lb != cache.end() && lb->first == x) return lb->second;
    return cache.insert(lb, {x, op(x)})->second;
  };
}

}  // namespace algo

#endif  // ALGO_MEMOIZED_FUNCTION_H
