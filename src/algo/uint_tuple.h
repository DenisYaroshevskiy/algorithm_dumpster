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

#ifndef ALGO_UINT_TUPLE_H
#define ALGO_UINT_TUPLE_H

#include "algo/binary_search.h"
#include "algo/type_functions.h"

#include <array>
#include <functional>
#include <stdexcept>

namespace algo {
namespace _uint_tuple {

template <typename I, typename T, typename Op>
constexpr T accumulate(I f, I l, T x, Op op) {
  while (f != l) {
    x = op(std::move(x), *f);
    ++f;
  }
  return x;
}

template <typename I, typename T>
constexpr T accumulate(I f, I l, T x) {
  return _uint_tuple::accumulate(f, l, x, std::plus<>{});
}

constexpr size_t round_to_possible_size(size_t x) {
  auto* found = algo::lower_bound(supported_uint_sizes.begin(),
                                  supported_uint_sizes.end(), x);
  if (found == supported_uint_sizes.end()) {
    // Will cause a compile time error
    throw std::range_error{"does not fit into an integer"};
  }
  return *found;
}

template <size_t N>
constexpr size_t select_size(std::array<size_t, N> sizes) {
  return round_to_possible_size(
    _uint_tuple::accumulate(sizes.begin(), sizes.end(), size_t{0})
  );
}

template <size_t ... N>
using selected_type = uint_t<select_size(std::array{N...})>;

}  // namespace _uint_tuple

template <size_t... N>
class uint_tuple {
 public:
  using storage_type = _uint_tuple::selected_type<N...>;
 private:
};

}  // namespace algo

#endif  // ALGO_UINT_TUPLE_H
