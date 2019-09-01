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

#ifndef ALGO_TYPE_FUNCTIONS_H
#define ALGO_TYPE_FUNCTIONS_H

#include <iterator>
#include <type_traits>

namespace algo {
namespace detail {

template <typename I>
struct is_move_iterator : std::false_type {};

template <typename I>
struct is_move_iterator<std::move_iterator<I>> : std::true_type {};

template <typename I>
struct is_reverse_iterator : std::false_type {};

template <typename I>
struct is_reverse_iterator<std::reverse_iterator<I>> : std::true_type {};

}  // namespace detail

struct n_iterator_tag {};

template <typename I>
using ValueType = typename std::iterator_traits<I>::value_type;

template <typename I>
using DifferenceType = typename std::iterator_traits<I>::difference_type;

template <typename I>
using Pointer = typename std::iterator_traits<I>::pointer;

template <typename I>
using Reference = typename std::iterator_traits<I>::reference;

template <typename I>
using IteratorCategory = typename std::iterator_traits<I>::iterator_category;

template <typename I>
constexpr bool ForwardIterator =
    std::is_base_of_v<std::forward_iterator_tag, IteratorCategory<I>>;

template <typename I>
constexpr bool BidirectionalIterator =
    std::is_base_of_v<std::bidirectional_iterator_tag, IteratorCategory<I>>;

template <typename I>
constexpr bool RandomAccessIterator =
    std::is_base_of_v<std::random_access_iterator_tag, IteratorCategory<I>>;

template <typename I>
constexpr bool MoveIterator = detail::is_move_iterator<I>::value;

template <typename I>
constexpr bool ReverseIterator = detail::is_reverse_iterator<I>::value;

}  // namespace algo

#endif  // ALGO_TYPE_FUNCTIONS_H
