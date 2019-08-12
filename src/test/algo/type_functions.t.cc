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

#include "algo/type_functions.h"

#include <array>
#include <iterator>
#include <list>
#include <vector>

#include "test/catch.h"

namespace algo {
namespace {

TEST_CASE("iterator categories", "[aglorithm]") {
  static_assert(RandomAccessIterator<int*>);
  static_assert(!RandomAccessIterator<std::list<int>::iterator>);
}

TEST_CASE("ReverseIterator", "[algorithm]") {
  static_assert(ReverseIterator<std::reverse_iterator<int*>>);
  static_assert(ReverseIterator<std::reverse_iterator<const int*>>);
  static_assert(ReverseIterator<std::array<int, 3>::reverse_iterator>);
  static_assert(ReverseIterator<std::array<int, 3>::const_reverse_iterator>);
  static_assert(ReverseIterator<std::vector<int>::reverse_iterator>);
  static_assert(ReverseIterator<std::vector<int>::const_reverse_iterator>);
  static_assert(ReverseIterator<std::list<int>::reverse_iterator>);
  static_assert(ReverseIterator<std::list<int>::const_reverse_iterator>);
}

}  // namespace
}  // namespace algo
