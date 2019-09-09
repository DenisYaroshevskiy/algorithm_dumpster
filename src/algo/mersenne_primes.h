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

#ifndef ALGO_MERSEN_PRIMES_H
#define ALGO_MERSEN_PRIMES_H

#include <array>
#include <cinttypes>
#include <limits>

namespace algo {

inline constexpr std::array<std::int32_t, 8> mersen_primes_int32 = {
    (1 << 2) - 1,   //
    (1 << 3) - 1,   //
    (1 << 5) - 1,   //
    (1 << 7) - 1,   //
    (1 << 13) - 1,  //
    (1 << 17) - 1,  //
    (1 << 19) - 1,  //
    std::numeric_limits<std::int32_t>::max()   //
};

}  // namespace algo

#endif  // ALGO_MERSEN_PRIMES_H
