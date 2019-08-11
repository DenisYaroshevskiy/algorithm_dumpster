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

#ifndef ALGO_COPY_H
#define ALGO_COPY_H

#include <type_traits>

#include "algo/type_functions.h"

namespace algo {
namespace detail {

template <bool is_backward, typename Traits, typename I, typename O>
O do_copy(I f, I l, O o) {
  using T = ValueType<I>;
  using U = ValueType<O>;

  constexpr bool is_trivial_copy = std::is_trivially_copy_assignable_v<U> &&
                                   std::is_same_v<std::remove_const_t<T>, U>;

  if constexpr (MoveIterator<I> && is_trivial_copy) {
    return O{do_copy<is_backward, Traits>(f.base(), l.base(), o)};
  } else if constexpr (MoveIterator<O>) {
    return O{do_copy<is_backward, Traits>(f, l, o.base())};
  } else if constexpr (ReverseIterator<I> && ReverseIterator<O>) {
    return O{do_copy<!is_backward, Traits>(l.base(), f.base(), o.base())};
  } else if constexpr (is_backward) {
    return O{Traits::copy_backward(f, l, o)};
  } else {
    return O{Traits::copy(f, l, o)};
  }
}

struct copy_runtime_traits {
  template <typename I, typename O>
  static O copy(I f, I l, O o) {
    return std::copy(f, l, o);
  }

  template <typename I, typename O>
  static O copy_backward(I f, I l, O o) {
    return std::copy_backward(f, l, o);
  }
};

}  // namespace detail

template <typename I, typename O>
// require ForwardIterator<I> && OutputIterator<O>
O copy(I f, I l, O o) {
  return detail::do_copy<false, detail::copy_runtime_traits>(f, l, o);
}

template <typename I, typename O>
// require BidirectionalIterator<I> && OutputIterator<O>
O copy_backward(I f, I l, O o) {
  return detail::do_copy<true, detail::copy_runtime_traits>(f, l, o);
}

}  // namespace algo

#endif  // ALGO_COPY_H
