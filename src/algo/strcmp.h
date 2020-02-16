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
/*
namespace _strcmp {

template <std::size_t width>
bool strmismatch_middle_of_the_page(const char*& x, const char*& y) {
  using pack = simd::pack<char, width>;
  using vbool = simd::vbool_t<pack>;

  const char* page_end_x = simd::end_of_page(x);
  const pack zeros = simd::setzero<pack>();
  const std::ptrdiff_t swidth = static_cast<std::ptrdiff_t>(width);

  while (true) {
    if (page_end_x - x < swidth) return false;

    const pack chars_x = simd::loadu<pack>(x);
    const pack chars_y = simd::loadu<pack>(y);

    const vbool end_test = simd::equal_pairwise(chars_x, zeros);
    const vbool x_eq_y = simd::equal_pairwise(chars_x, chars_y);

    const std::optional missmatch = first_true(x_eq_y);
    const std::optional x_zero = first_true(end_test));

    if (!x_zero && !missmatch) continue;

    std::uint32_t offset;

    if (!x_zero) offset = *missmatch;
    else if (!missmatch) offset = *x_zero;
    else offset = std::min(*x_zero, *missmatch);

    x += offset;
    y += offset;
    return true;
  }
}

}  // namespace _strcmp
*/

template <std::size_t width>
std::pair<const char*, const char*> strmismatch(const char* x, const char* y) {
  while (*x && *x == *y) {
    ++x;
    ++y;
  }

  return {x, y};
}

template <std::size_t width>
int strcmp(const char* sx, const char* sy) {
  auto [x, y] = strmismatch<width>(sx, sy);

  unsigned ux = static_cast<unsigned>(*x);
  unsigned uy = static_cast<unsigned>(*y);
  if (ux < uy) return -1;
  if (ux == uy) return 0;
  return 1;
}

}  // namespace algo

#endif  // ALGO_STRCMP_H
