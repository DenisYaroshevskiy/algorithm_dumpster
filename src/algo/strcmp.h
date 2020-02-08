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

#include "simd/pack.h"

namespace algo {

#if 0
template <std::size_t width>
int strcmp(const char* x, const char* y) {
  using pack = simd::pack<char, width>;
  using vbool = simd::vbool_t<pack>;

  const pack zeros = simd::set_zero<pack>();

  auto [x_chars, x_aligned] = simd::load_left_align<width>(sx);
  auto [y_chars, y_aligned] = simd::load_left_align<width>(sy);

  const std::uint32_t x_offset = static_cast<std::uint32_t>(x - x_aligned);
  const std::uint32_t y_offset = static_cast<std::uint32_t>(y - y_aligned);

  vbool_t x_end_test = simd::equal_pairwise(x_chars, zeros);



}
#endif  //

}  // namespace algo

#endif  // ALGO_STRCMP_H
