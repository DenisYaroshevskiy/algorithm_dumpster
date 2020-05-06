/*
 * Copyright 2020 Denis Yaroshevskiy
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

#ifndef SIMD_PACK_DETAIL_REDUCE_H_
#define SIMD_PACK_DETAIL_REDUCE_H_

#include "simd/pack_detail/pack_declaration.h"
#include "simd/pack_detail/shuffle.h"

namespace simd {
namespace _reduce {

// Reduction for 4 numbers. We have to commute:
//
// [0, 1, 2, 3] + [2, 3, 0, 1] =
//    [0 + 2, 1 + 3, 2 + 0, 1 + 3]
// [0 + 2, 1 + 3, 2 + 0, 1 + 3] + [1 + 3, 0 + 2, 1 + 3, 0 + 2] =
//    [0 + 1 + 2 + 3, 0 + 1 + 2 + 3, 0 + 1 + 2 + 3, 0 + 1 + 2 + 3]

template <std::size_t current_width, typename Pack, typename Op>
Pack reduce_impl(Pack x, Op op) {
  Pack y = swap_adjacent_groups<current_width>(x);
  x = op(x, y);
  if constexpr (current_width == 1) {
    return x;
  } else {
    return reduce_impl<current_width / 2>(x, op);
  }
}

}  // namespace _reduce

template <typename T, std::size_t W, typename Op>
// require BinaryTransformation<Op, pack<T, W>>
pack<T, W> reduce(const pack<T, W>& x, Op op) {
  return _reduce::reduce_impl<W / 2>(x, op);
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_REDUCE_H_
