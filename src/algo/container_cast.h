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

#ifndef ALGO_CONTAINER_CAST_H
#define ALGO_CONTAINER_CAST_H

#include <iterator>
#include <type_traits>

namespace algo {

template <template <typename...> class C, typename InC>
// require Container<C>, Container<InC>
auto container_cast(InC&& in) {
  using T = typename std::decay_t<InC>::value_type;
  if constexpr (std::is_rvalue_reference_v<decltype(in)>) {
    return C<T>(std::make_move_iterator(in.begin()),
                std::make_move_iterator(in.end()));
  } else {
    return C<T>(in.begin(), in.end());
  }
}

}  // namespace algo

#endif  // ALGO_CONTAINER_CAST_H
