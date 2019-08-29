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

#ifndef TEST_STABILITY_TEST_UTIL_H
#define TEST_STABILITY_TEST_UTIL_H

#include <algorithm>
#include <iostream>
#include <iterator>
#include <utility>
#include <vector>

#include "test/algo/zeroed_int.h"

namespace algo {

using stable_unique = std::pair<zeroed_int, int>;

inline std::ostream& operator<<(std::ostream& out, const stable_unique& x) {
  return out << '{' << x.first << ',' << x.second << '}';
}

template <template <typename...> class ToC, typename C>
auto cast_container_of_stable_unique(const C& c) {
  ToC<stable_unique> res;
  std::transform(c.begin(), c.end(), std::inserter(res, res.end()),
                 [&](const stable_unique& x) {
                   return stable_unique{zeroed_int(x.first.body), x.second};
                 });
  return res;
}

template <typename C>
auto copy_container_of_stable_unique(const C& c) {
  C res;
  std::transform(c.begin(), c.end(), std::inserter(res, res.end()),
                 [&](const stable_unique& x) {
                   return stable_unique{zeroed_int(x.first.body), x.second};
                 });
  return res;
}

template <template <typename...> class C>
auto make_container_of_stable_unique(const std::vector<int>& values, int tag) {
  C<stable_unique> res;
  std::transform(values.begin(), values.end(), std::inserter(res, res.end()),
                 [&](int x) {
                   return stable_unique{zeroed_int(x), tag};
                 });
  return res;
}

template <template <typename...> class C>
auto make_container_of_stable_unique_iota(const std::vector<int>& values) {
  int tag = 0;
  C<stable_unique> res;
  std::transform(values.begin(), values.end(), std::inserter(res, res.end()),
                 [&](int x) mutable {
                   return stable_unique{zeroed_int(x), ++tag};
                 });
  return res;
}

}  // namespace algo

#endif  // TEST_STABILITY_TEST_UTIL_H
