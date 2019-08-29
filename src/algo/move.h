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

#ifndef ALGO_MOVE_H
#define ALGO_MOVE_H

#include "algo/copy.h"

namespace algo {

template <typename I, typename O>
// require InputIterator<I> && OutputIterator<O>
O move(I f, I l, O o) {
  return algo::copy(std::move_iterator<I>(f), std::move_iterator<I>(l), o);
}

template <typename I, typename O>
// require BidirectionalIterator<I> && BidirectionalIterator<O>
O move_backward(I f, I l, O o) {
  return algo::copy_backward(std::move_iterator<I>(f), std::move_iterator<I>(l), o);
}

template <typename I, typename N, typename O>
// require InputIterator<I> && Number<N> && OutputIterator<O>
std::pair<I, O> move_n(I f, N n, O o) {
  auto [move_l, o_l] = algo::copy_n(std::move_iterator<I>(f), n, o);
  return {move_l.base(), o_l};
}

template <typename I, typename N, typename O>
// require BidirectionalIterator<I> && Number<N> && BidirectionalIterator<O>
std::pair<I, O> move_backward_n(I l, N n, O o) {
  auto [move_f, o_f] = algo::copy_backward_n(std::move_iterator<I>(l), n, o);
  return {move_f.base(), o_f};
}

}  // namespace algo

#endif  // ALGO_MOVE_H
