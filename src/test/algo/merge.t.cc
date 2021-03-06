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

#include "algo/merge.h"

#include "test/catch.h"

#include "test/algo/merge_generic_test.h"

namespace algo {
namespace {

TEST_CASE("merge_expensive_cmp", "[algorithm]") {
  merge_test([](auto... params) { algo::merge_expensive_cmp(params...); });
}

TEST_CASE("merge", "[algorithm]") {
  merge_test([](auto... params) { algo::merge(params...); });
}

}  // namespace
}  // namespace algo
