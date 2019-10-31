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
#include <array>
#include <cstddef>
#include <cstdint>
#include <random>
#include <set>
#include <utility>
#include <vector>

#include <boost/multiprecision/cpp_int.hpp>

#include "algo/factorial.h"
#include "algo/memoized_function.h"
#include "algo/nth_permutation.h"
#include "algo/shuffle_biased.h"
#include "algo/type_functions.h"
#include "bench_generic/fake_url.h"
#include "bench_generic/noinline_int.h"

namespace bench {

using std_int64_t = std::int64_t;

using fake_url_pair = std::pair<fake_url, fake_url>;

template <typename T>
struct int_to_t {
  std::enable_if_t<std::is_integral_v<T>, T> operator()(int x) const {
    return x;
  }
};

template <>
struct int_to_t<double> {
  double operator()(int x) const { return 1.0 / x; }
};

template <>
struct int_to_t<fake_url> {
  fake_url operator()(int x) const { return fake_url(x); }
};

template <>
struct int_to_t<fake_url_pair> {
  fake_url_pair operator()(int x) const {
    return std::make_pair(fake_url(x), fake_url(x));
  }
};

template <>
struct int_to_t<noinline_int> {
  noinline_int operator()(int x) const { return noinline_int{x}; }
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
auto generate_random_vector(size_t size, Src src) {
  const int_to_t<T> to_t;

  std::vector<T> res(size);
  std::generate(res.begin(), res.end(), [&] { return to_t(src()); });

  return res;
}

template <typename T, typename Src>
auto generate_sorted_vector(size_t size, Src src) {
  auto res = generate_random_vector<T>(size, src);
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
std::vector<T> random_vector(size_t size) {
  using namespace detail;

  static auto gen = algo::memoized_function<size_t>([](size_t size) {
    return generate_random_vector<T>(size, uniform_src(size));
  });

  return gen(size);
}

template <typename T>
std::vector<T> sorted_vector(size_t size) {
  using namespace detail;

  static auto gen = algo::memoized_function<size_t>([](size_t size) {
    return generate_sorted_vector<T>(size, uniform_src(size));
  });

  return gen(size);
}

template <typename T>
std::pair<std::vector<T>, std::vector<T>> two_random_vectors(size_t x_size,
                                                             size_t y_size) {
  using namespace detail;

  static auto gen = algo::memoized_function<std::pair<size_t, size_t>>(
      [](std::pair<size_t, size_t> sizes) {
        auto src = uniform_src(sizes.first + sizes.second);
        return std::make_pair(generate_random_vector<T>(sizes.first, src),
                              generate_random_vector<T>(sizes.second, src));
      });

  return gen({x_size, y_size});
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

template <typename T>
std::vector<T> nth_vector_permutation(size_t size, int percentage) {
  auto sorted_vec = sorted_vector<T>(size);

  using big_int = boost::multiprecision::cpp_int;
  const big_int selected_permutation =
      (algo::factorial<big_int>(static_cast<int>(size)) - 1) * percentage / 100;

  std::vector<T> vec(size);
  algo::nth_permutation(sorted_vec.begin(), sorted_vec.end(), vec.begin(),
                        selected_permutation);

  return vec;
}

template <typename Base>
auto shuffled_vector(size_t size, int percentage, Base base) {
  const int left_percentage = percentage > 50 ? 100 - percentage : percentage;

  static auto gen = algo::memoized_function<std::pair<size_t, int>>(
      [base](std::pair<size_t, int> param) {
        auto [size, left_percentage] = param;
        auto vec = base(size);

        int biased_limit = static_cast<int>(size) * left_percentage / 50;
        if (biased_limit == 0) biased_limit = 1;
        algo::shuffle_biased(vec.begin(), vec.end(), biased_limit,
                             detail::static_generator());

        return vec;
      });

  auto vec = gen({size, left_percentage});
  if (percentage > 50) std::reverse(vec.begin(), vec.end());
  return vec;
}

template <typename T>
auto shuffled_positions(std::vector<T>& data, size_t size, int percentage) {
  auto shuffle_as = bench::shuffled_vector(size, percentage, [](size_t size) {
    std::vector<int> idxes(size);
    std::iota(idxes.begin(), idxes.end(), 0);
    return idxes;
  });

  std::vector<T> opt_output(size);

  using I = typename std::vector<T>::iterator;
  std::vector<I> positions(size);
  std::transform(shuffle_as.begin(), shuffle_as.end(), positions.begin(),
                 [&](int idx) { return data.begin() + idx; });
  return positions;
}

}  // namespace bench

#endif  // BENCH_GENERIC_INPUT_GENERATORS_H
