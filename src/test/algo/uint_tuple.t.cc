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

#include "algo/uint_tuple.h"

#include <type_traits>

#include "test/catch.h"

namespace algo {
namespace {

template <typename T, size_t ... N>
constexpr void storage_type_test() {
  static_assert(std::is_same_v<T, typename uint_tuple<N...>::storage_type>);
}

TEST_CASE("algorithm.uint_tuple.type", "[algorithm]") {
  storage_type_test<uint8_t, 1>();
  storage_type_test<uint8_t, 4, 4>();
  storage_type_test<uint8_t, 8>();

  storage_type_test<uint16_t, 8, 8>();
  storage_type_test<uint16_t, 16>();

  storage_type_test<uint32_t, 8, 8, 8>();
  storage_type_test<uint32_t, 32>();

  storage_type_test<uint64_t, 32, 16, 16>();
  storage_type_test<uint64_t, 64>();

#ifdef HAS_128_INTS
  storage_type_test<algo::uint_t<128>, 64, 64>();
#endif  // HAS_128_INTS
}

}  // namespace
}  // namespace algo
