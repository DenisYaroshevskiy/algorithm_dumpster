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

#ifndef BENCH_GENERIC_INPUT_GENERATORS_H
#define BENCH_GENERIC_INPUT_GENERATORS_H

#include <algorithm>
#include <cstddef>
#include <random>
#include <set>
#include <utility>
#include <vector>

#include "algo/memoized_function.h"

namespace bench {

template <typename>
struct int_to_t;

template <>
struct int_to_t<int> {
  int operator()(int x) const { return x; }
};

namespace detail {

template <typename T, typename Src>
auto generate_unique_sorted_vector(size_t size, Src src) {
  const int_to_t<T> to_t;
  std::set<T> values;
  while (values.size() < size) {
    values.insert(to_t(src()));
  }
  return std::vector<T>(values.begin(), values.end());
}

template <typename T, typename Src>
auto generate_sorted_vector(size_t size, Src src) {
  const int_to_t<T> to_t;

  std::vector<T> res(size);
  std::generate(res.begin(), res.end(), [&] { return to_t(src()); });
  std::sort(res.begin(), res.end());

  return res;
}

std::mt19937& static_generator() {
  static std::mt19937 g;
  return g;
}

auto uniform_src(size_t size) {
  return [ud = std::uniform_int_distribution<>{
              1, static_cast<int>(size) * 20}]() mutable {
    return ud(static_generator());
  };
}

}  // namespace detail

template <typename T>
std::vector<T> sorted_vector(size_t size) {
  using namespace detail;

  static auto gen = algo::memoized_function<size_t>([](size_t size) {
    return generate_sorted_vector<T>(size, uniform_src(size));
  });

  return gen(size);
}

template <typename T>
std::pair<std::vector<T>, std::vector<T>> two_sorted_vectors(size_t x_size,
                                                             size_t y_size) {
  using namespace detail;

  static auto gen = algo::memoized_function<std::pair<size_t, size_t>>(
      [](std::pair<size_t, size_t> sizes) {
        auto src = uniform_src(sizes.first + sizes.second);
        return std::make_pair(generate_sorted_vector<T>(sizes.first, src),
                              generate_sorted_vector<T>(sizes.second, src));
      });

  return gen({x_size, y_size});
}

}  // namespace bench

#endif  // BENCH_GENERIC_INPUT_GENERATORS_H
