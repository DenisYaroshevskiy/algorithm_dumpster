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

#ifndef SIMD_PACK_DETAIL_ADDRESS_MANIPULATION_H_
#define SIMD_PACK_DETAIL_ADDRESS_MANIPULATION_H_

#include <cstddef>

namespace simd {

constexpr std::ptrdiff_t page_size() { return 1 << 12; }

template <typename T>
T* end_of_page(T* addr) {
  std::uintptr_t upage_size = page_size();
  std::uintptr_t mask = ~(upage_size - 1);
  return reinterpret_cast<T*>((reinterpret_cast<std::uintptr_t>(addr) & mask) +
                              upage_size);
}

template <typename Pack, typename T>
T* previous_aligned_address(T* addr) {
  constexpr std::uintptr_t mask = ~(alignof(Pack) - 1);
  return reinterpret_cast<T*>(reinterpret_cast<std::uintptr_t>(addr) & mask);
}

}  // namespace simd

#endif  // SIMD_PACK_DETAIL_ADDRESS_MANIPULATION_H_
