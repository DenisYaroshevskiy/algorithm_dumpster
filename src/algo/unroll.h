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

#ifndef ALGO_UNROLL_H
#define ALGO_UNROLL_H

#include <cstddef>
#include <utility>

namespace algo {
namespace detail {

template <typename Op, size_t... from0_to_n>
constexpr void unroll_impl(Op op, std::index_sequence<from0_to_n...>) {
  (((void)from0_to_n, op()), ...);
}

}  // namespace detail

template <size_t N, typename Op>
// require Callable<Op>
constexpr void unroll(Op op) {
  detail::unroll_impl(op, std::make_index_sequence<N>{});
}

}  // namespace algo

#endif  // ALGO_UNROLL_H
