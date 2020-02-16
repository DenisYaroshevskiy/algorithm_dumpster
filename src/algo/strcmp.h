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

#ifndef ALGO_STRCMP_H
#define ALGO_STRCMP_H

#include <algorithm>
#include <utility>

#include "simd/pack.h"

namespace algo {
namespace _strcmp {

template <std::size_t width>
bool strmismatch_middle_of_the_page(std::ptrdiff_t n, const char** px,
                                    const char** py) {
  using pack = simd::pack<char, width>;
  using vbool = simd::vbool_t<pack>;

  const char*& x = *px;
  const char*& y = *py;

  const pack zeros = simd::set_zero<pack>();

  while (n) {
    const pack chars_x = simd::load_unaligned<pack>(x);
    const pack chars_y = simd::load_unaligned<pack>(y);

    const vbool end_test = simd::equal_pairwise(chars_x, zeros);
    const vbool x_neq_y = ~simd::equal_pairwise(chars_x, chars_y);
    const vbool test = end_test | x_neq_y;

    std::optional stop_at = first_true(test);
    if (!stop_at) {
      x += width;
      y += width;
      --n;
      continue;
    }

    x += *stop_at;
    y += *stop_at;
    return true;
  }
  return false;
}

inline bool strmismatch_page_boundary(std::ptrdiff_t n, const char** px,
                                      const char** py) {
  const char*& x = *px;
  const char*& y = *py;

  while (n) {
    if (!*x || *x != *y) return true;
    ++x;
    ++y;
    --n;
  }
  return false;
}

}  // namespace _strcmp

template <std::size_t width>
std::pair<const char*, const char*> strmismatch(const char* x, const char* y) {
  const char* page_end_x = simd::end_of_page(x);
  const char* page_end_y = simd::end_of_page(y);
  std::ptrdiff_t n = std::min(page_end_x - x, page_end_y - y);
  std::ptrdiff_t simd_n = n / width;
  n -= simd_n * width;

  if (_strcmp::strmismatch_middle_of_the_page<width>(simd_n, &x, &y) ||
      _strcmp::strmismatch_page_boundary(n, &x, &y)) {
    return {x, y};
  }
  return strmismatch<width>(x, y);
}

template <std::size_t width>
int strcmp(const char* sx, const char* sy) {
  auto [x, y] = strmismatch<width>(sx, sy);

  auto as_int = [](const char x) {
    return static_cast<int>(static_cast<unsigned char>(x));
  };

  return as_int(*x) - as_int(*y);
}

}  // namespace algo

#endif  // ALGO_STRCMP_H
