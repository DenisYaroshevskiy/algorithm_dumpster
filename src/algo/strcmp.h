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

#include <utility>

#include "simd/pack.h"

namespace algo {

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
