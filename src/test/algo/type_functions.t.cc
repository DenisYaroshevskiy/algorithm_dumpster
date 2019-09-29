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
#include <functional>

#include "test/catch.h"

namespace algo {
namespace {

TEST_CASE("algorithm.iterator_categories", "[aglorithm]") {
  static_assert(RandomAccessIterator<int*>);
  static_assert(!RandomAccessIterator<std::list<int>::iterator>);
}

TEST_CASE("algorithm.ReverseIterator", "[algorithm]") {
  static_assert(ReverseIterator<std::reverse_iterator<int*>>);
  static_assert(ReverseIterator<std::reverse_iterator<const int*>>);
  static_assert(ReverseIterator<std::array<int, 3>::reverse_iterator>);
  static_assert(ReverseIterator<std::array<int, 3>::const_reverse_iterator>);
  static_assert(ReverseIterator<std::vector<int>::reverse_iterator>);
  static_assert(ReverseIterator<std::vector<int>::const_reverse_iterator>);
  static_assert(ReverseIterator<std::list<int>::reverse_iterator>);
  static_assert(ReverseIterator<std::list<int>::const_reverse_iterator>);
}

template <typename T>
void is_same_test(T, T){}

template <typename T, typename F>
void argument_type_test(F) {
  is_same_test(type_t<T>{}, type_t<ArgumentType<F>>{});
}

TEST_CASE("algorithm.argument_type", "[algorithm]") {
  argument_type_test<int>([](int){});
  argument_type_test<int&>([](int&){});
  argument_type_test<const int&>([](const int&){});

  struct non_const_callable {
    void operator()(int) {}
  } s;

  argument_type_test<int>(s);
  argument_type_test<const int&>(std::plus<int>{});
}

template <size_t N, typename = void>
struct assert_no_such_uint_t {};

template <size_t N>
struct assert_no_such_uint_t<N, std::void_t<uint_t<N>>>;

TEST_CASE("algorithm.uint_t", "[algorithm]") {
  is_same_test(uint_t<8>{}, std::uint8_t{});
  is_same_test(uint_t<16>{}, std::uint16_t{});
  is_same_test(uint_t<32>{}, std::uint32_t{});
  is_same_test(uint_t<64>{}, std::uint64_t{});
#ifdef HAS_128_INTS
  is_same_test(uint_t<128>{}, __uint128_t{});
#endif

  (void)assert_no_such_uint_t<10>{};
}

}  // namespace
}  // namespace algo
