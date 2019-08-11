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

#include "algo/factorial.h"

#include "test/catch.h"

namespace algo {
namespace {

TEST_CASE("algorithm.factorial", "[algorithm]") {
  static_assert(1 == factorial<int>(0));
  static_assert(1 == factorial<int>(1));
  static_assert(2 == factorial<int>(2));
  static_assert(6 == factorial<int>(3));
  static_assert(24 == factorial<int>(4));
}

}  // namespace
}  // namespace algo
