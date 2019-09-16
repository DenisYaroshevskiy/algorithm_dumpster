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

#ifndef BENCH_GENERIC_NOINLINE_INT_H
#define BENCH_GENERIC_NOINLINE_INT_H

#include "compiler/compiler_directives.h"

namespace bench {

struct noinline_int {
  int data;

  noinline_int();
  noinline_int(const noinline_int&);
  noinline_int(noinline_int&&) noexcept;
  noinline_int& operator=(const noinline_int&);
  noinline_int& operator=(noinline_int&&) noexcept;

  explicit noinline_int(int seed) : data(seed) {}

  friend bool operator==(const noinline_int& x, const noinline_int& y);

  friend bool operator<(const noinline_int& x, const noinline_int& y);

  friend bool operator!=(const noinline_int& x, const noinline_int& y) {
    return !(x == y);
  }

  friend bool operator>(const noinline_int& x, const noinline_int& y) {
    return y < x;
  }

  friend bool operator<=(const noinline_int& x, const noinline_int& y) {
    return !(y < x);
  }

  friend bool operator>=(const noinline_int& x, const noinline_int& y) {
    return !(x < y);
  }
};

ALGO_NOINLINE noinline_int::noinline_int() = default;
ALGO_NOINLINE noinline_int::noinline_int(const noinline_int&) = default;
ALGO_NOINLINE noinline_int::noinline_int(noinline_int&&) noexcept = default;
ALGO_NOINLINE noinline_int& noinline_int::operator=(const noinline_int&) = default;
ALGO_NOINLINE noinline_int& noinline_int::operator=(noinline_int&&) noexcept = default;

template <typename H>
ALGO_NOINLINE H AbslHashValue(H h, const noinline_int& x) {
  return H::combine(std::move(h), x.data);
}

ALGO_NOINLINE bool operator==(const noinline_int& x, const noinline_int& y) {
  return x.data == y.data;
}

ALGO_NOINLINE bool operator<(const noinline_int& x, const noinline_int& y) {
  return x.data < y.data;
}

}  // namespace bench

#endif  // BENCH_GENERIC_NOINLINE_INT_H
