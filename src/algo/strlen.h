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

#include "algo/simd.h"

namespace algo {

inline size_t strlen(const char* s_chars) {
  const auto* s = reinterpret_cast<const std::int8_t*>(s_chars);

  simd<std::int8_t, 16> zeros;
  zeros.fill_0();

  auto chars = load_unaligned_with_filler<simd<std::int8_t, 16>>(s, 0xff);

  constexpr std::uintptr_t mask = ~15;

  const auto* aligned_s =
      reinterpret_cast<decltype(s)>(reinterpret_cast<std::uintptr_t>(s) & mask);

  while (!any_pairwise_equal(chars, zeros)) {
    aligned_s += 16;
    chars.load(aligned_s);
  }

  return static_cast<size_t>((aligned_s + first_pairwise_equal(chars, zeros)) -
                             s);
}

}  // namespace algo

#endif  // ALGO_STRLEN_H
