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

#ifndef ALGO_STRLEN_H
#define ALGO_STRLEN_H

#include "simd/pack.h"

namespace algo {

template <std::size_t width>
std::size_t strlen(const char* s) {
  using pack = simd::pack<char, width>;
  using vbool = simd::vbool_t<pack>;

  const pack zeros = simd::set_zero<pack>();

  const char* aligned_s = simd::previous_aligned_address<pack>(s);
  auto chars = simd::load<pack>(aligned_s);

  const std::uint32_t offset = static_cast<std::uint32_t>(s - aligned_s);

  vbool test = simd::equal_pairwise(chars, zeros);
  std::optional match = simd::first_true_ignore_first_n(test, offset);

  while (!match) {
    aligned_s += width;
    chars = simd::load<pack>(aligned_s);
    test = simd::equal_pairwise(chars, zeros);
    match = simd::first_true(test);
  }

  return static_cast<size_t>(aligned_s + *match - s);
}

}  // namespace algo

#endif  // ALGO_STRLEN_H
