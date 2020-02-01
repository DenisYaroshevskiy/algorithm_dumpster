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

#ifndef SIMD_PACK_DETAIL_MASKS_H_
#define SIMD_PACK_DETAIL_MASKS_H_

#include <cstdint>

namespace simd {

constexpr std::int32_t lower_n_bits_1(std::int32_t n) {
  std::int64_t res{1};
  res <<= n;
  res -= 1;
  return static_cast<std::int32_t>(res);
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_MASKS_H_
