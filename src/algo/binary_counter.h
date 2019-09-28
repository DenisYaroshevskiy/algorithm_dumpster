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

#ifndef BINARY_COUNTER_H
#define BINARY_COUNTER_H

#include <array>
#include <iterator>
#include <utility>

#include <iostream>

#include "algo/type_functions.h"

namespace algo {

template <typename I, typename Op, typename U>
// require ForwardIterator<I> && Regular<ValueType<I>>
//         Callable<Op, ValueType<I>(ValueType<I>&&, ValueType<I>&&)> &&
//         Associative<Op> && Compatible<T, U>
constexpr ValueType<I> add_to_counter(I f, I l, Op op, const U& zero,
                                      ValueType<I> carry) {
  while (f != l) {
    if (*f == zero) {
      *f = std::move(carry);
      return ValueType<I>(zero);
    }

    carry = op(std::move(*f), std::move(carry));
    *f = ValueType<I>(zero);
    ++f;
  }

  return carry;
}

template <typename I, typename Op, typename U>
// require ForwardIterator<I> && Regular<ValueType<I>>
//         Callable<Op, ValueType<I>(ValueType<I>&&, ValueType<I>&&)> &&
//         Associative<Op> && Compatible<T, U>
constexpr ValueType<I> reduce_counter(I f, I l, Op op, const U& zero) {
  while (true) {
    if (f == l) return ValueType<I>(zero);
    if (*f != zero) break;
    ++f;
  }

  ValueType<I> r = std::move(*f);
  while (true) {
    ++f;
    if (f == l) return r;
    if (*f == zero) continue;
    r = op(std::move(*f), std::move(r));
  }

  return r;
}

template <typename I, typename U>
constexpr I last_significant_digit(I f, I l, const U& zero) {
  return std::find_if_not(std::reverse_iterator<I>(l),
                          std::reverse_iterator<I>(f),
                          [&](const auto& x) { return x == zero; })
      .base();
}

template <size_t N, typename Op, typename T = std::decay_t<ArgumentType<Op>>,
          typename U = T>
class binary_counter_fixed : Op {
  std::array<T, N> body_;
  std::ptrdiff_t size_ = 0;
  U zero_;

 public:
  using value_type = T;
  using zero_type = U;
  using reference = T&;
  using const_reference = const T&;
  using operation_type = Op;

  inline static constexpr size_t digits = N;

  using iterator = T*;
  using const_iterator = const T*;
  using reverse_iterator = std::reverse_iterator<T*>;
  using const_reverse_iterator = std::reverse_iterator<T*>;

  constexpr binary_counter_fixed(operation_type op, zero_type zero)
      : Op(op), zero_(std::move(zero)) {}

  constexpr void reserve(size_t n) {
    std::ptrdiff_t n_signed = static_cast<std::ptrdiff_t>(n);
    std::ptrdiff_t adjust = n_signed - size_;
    if (adjust <= 0) return;
    std::fill_n(begin() + size_, adjust, zero_);
    size_ = n_signed;
  }

  operation_type operation() const { return *this; }

  constexpr void add(value_type x) {
    x = add_to_counter(begin(), end(), operation(), zero_, std::move(x));
    if (x == zero_) return;
    begin()[size_++] = std::move(x);
  }

  constexpr value_type reduce() {
    return reduce_counter(begin(), end(), operation(), zero_);
  }

  constexpr std::pair<iterator, iterator> significant_digits() {
    return {begin(), last_significant_digit(begin(), end(), zero_)};
  }

  constexpr std::pair<const_iterator, const_iterator> significant_digits()
      const {
    return {begin(), last_significant_digit(begin(), end(), zero_)};
  }

  constexpr iterator begin() { return body_.begin(); }
  constexpr const_iterator begin() const { return body_.begin(); }
  constexpr const_iterator cbegin() const { return body_.begin(); }

  constexpr iterator end() { return begin() + size_; }
  constexpr const_iterator end() const { return begin() + size_; }
  constexpr const_iterator cend() const { return end(); }
};

}  // namespace algo

#endif  // BINARY_COUNTER_H
