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

#include "algo/copy.h"

#include <array>
#include <list>
#include <string>

#include "algo/container_cast.h"

#include "test/catch.h"

namespace algo {
namespace {

template <typename ExpectedI, typename ExpectedO>
struct fwd {
  template <typename ActualI, typename ActualO>
  static ActualO copy(ActualI, ActualI, ActualO o) {
    static_assert(std::is_same_v<ExpectedI, ActualI>);
    static_assert(std::is_same_v<ExpectedO, ActualO>);
    return o;
  }
};

template <typename ExpectedI, typename ExpectedO>
struct back {
  template <typename ActualI, typename ActualO>
  static ActualO copy_backward(ActualI, ActualI, ActualO o) {
    static_assert(std::is_same_v<ExpectedI, ActualI>);
    static_assert(std::is_same_v<ExpectedO, ActualO>);
    return o;
  }
};

template <bool is_backward, typename Traits, typename I, typename O>
void test_do_copy() {
  auto res = algo::detail::do_copy<is_backward, Traits>(I(), I(), O());
  static_assert(std::is_same_v<O, decltype(res)>);
}

TEST_CASE("algorithm.copy.selection", "[algorithm]") {
  using algo::detail::do_copy;

  {
    using I = int*;
    using O = int*;

    test_do_copy<false, fwd<I, O>, I, O>();
    test_do_copy<true, back<I, O>, I, O>();
  }
  {
    using I = const int*;
    using O = int*;

    test_do_copy<false, fwd<I, O>, I, O>();
    test_do_copy<true, back<I, O>, I, O>();
  }
  {
    using ResI = int*;
    using I = std::move_iterator<ResI>;
    using O = int*;

    test_do_copy<false, fwd<ResI, O>, I, O>();
    test_do_copy<true, back<ResI, O>, I, O>();
  }
  {
    using I = int*;
    using O = std::move_iterator<int*>;
    using ResO = int*;

    test_do_copy<false, fwd<I, ResO>, I, O>();
    test_do_copy<true, back<I, ResO>, I, O>();
  }
  {
    using I = std::move_iterator<int*>;
    using O = std::move_iterator<int*>;
    using ResI = int*;
    using ResO = int*;

    test_do_copy<false, fwd<ResI, ResO>, I, O>();
    test_do_copy<true, back<ResI, ResO>, I, O>();
  }
  {
    using I = std::move_iterator<std::reverse_iterator<const int*>>;
    using O = std::move_iterator<int*>;
    using ResI = std::reverse_iterator<const int*>;
    using ResO = int*;

    test_do_copy<false, fwd<ResI, ResO>, I, O>();
    test_do_copy<true, back<ResI, ResO>, I, O>();
  }
  {
    using I = std::reverse_iterator<const int*>;
    using O = std::reverse_iterator<int*>;
    using ResI = const int*;
    using ResO = int*;

    test_do_copy<false, back<ResI, ResO>, I, O>();
    test_do_copy<true, fwd<ResI, ResO>, I, O>();
  }
  {
    using I = std::move_iterator<std::reverse_iterator<int*>>;
    using O = std::reverse_iterator<std::move_iterator<int*>>;
    using ResI = int*;
    using ResO = int*;

    test_do_copy<false, back<ResI, ResO>, I, O>();
    test_do_copy<true, fwd<ResI, ResO>, I, O>();
  }
  {
    using I = std::reverse_iterator<std::reverse_iterator<int*>>;
    using O = std::reverse_iterator<std::reverse_iterator<int*>>;
    using ResI = int*;
    using ResO = int*;

    test_do_copy<false, fwd<ResI, ResO>, I, O>();
    test_do_copy<true, back<ResI, ResO>, I, O>();
  }
  {
    using I = std::move_iterator<std::list<int>::const_iterator>;
    using O = std::list<int>::iterator;
    using ResI = std::list<int>::const_iterator;

    test_do_copy<false, fwd<ResI, O>, I, O>();
    test_do_copy<true, back<ResI, O>, I, O>();
  }
  {
    using I = std::move_iterator<std::list<int>*>;
    using O = std::list<int>*;

    test_do_copy<false, fwd<I, O>, I, O>();
    test_do_copy<true, back<I, O>, I, O>();
  }
}

/*
 Overlaps are tests for technically undefined behaviour:
 However - I wanted to check that the correct loops are selected
 and they go into the correct direction.
*/

TEST_CASE("algorithm.copy.overlaps.specific", "[algorithm]") {
  using namespace std::literals;
  const std::array init{"1"s, "2"s, "3"s, "4"s, "5"s, "6"s};

  {
    const std::array expected{"1"s, "2"s, "1"s, "2"s, "1"s, "2"s};

    std::array actual_std = init;
    std::copy(actual_std.begin(), actual_std.begin() + 4,
              actual_std.begin() + 2);

    REQUIRE(expected == actual_std);  // sanity check test correctness

    // copy
    {
      std::array actual = init;
      algo::copy(actual.begin(), actual.begin() + 4, actual.begin() + 2);

      REQUIRE(expected == actual);
    }

    // copy_n
    {
      std::array actual = init;
      algo::copy_n(actual.begin(), 4, actual.begin() + 2);

      REQUIRE(expected == actual);
    }
  }
  {
    const std::array expected{"1"s, "2"s, "1"s, "2"s, "3"s, "4"s};

    std::array actual_std = init;
    std::copy_backward(actual_std.begin(), actual_std.begin() + 4,
                       actual_std.end());

    REQUIRE(expected == actual_std);  // sanity check test correctness

    // copy_backward
    {
      std::array actual = init;
      algo::copy_backward(actual.begin(), actual.begin() + 4, actual.end());

      REQUIRE(expected == actual);
    }

    // copy_backward_n
    {
      std::array actual = init;
      algo::copy_backward_n(actual.begin() + 4, 4, actual.end());

      REQUIRE(expected == actual);
    }
  }
}

TEST_CASE("algorithm.copy.overlaps.running", "[algorithm]") {
  using namespace std::literals;
  const std::array init{"1"s, "2"s, "3"s, "4"s, "5"s, "6"s};

  auto test_fwd = [](auto arr, int n, int where) {
    auto expected = arr;
    std::copy(expected.begin(), expected.begin() + n, expected.begin() + where);

    // copy
    {
      auto actual_fwd = arr;
      algo::copy(actual_fwd.begin(), actual_fwd.begin() + n,
                 actual_fwd.begin() + where);

      REQUIRE(expected == actual_fwd);
    }

    // copy_n
    {
      auto actual_fwd = arr;
      algo::copy_n(actual_fwd.begin(), n, actual_fwd.begin() + where);

      REQUIRE(expected == actual_fwd);
    }

    // copy_backward
    {
      auto actual_back = arr;
      algo::copy_backward(actual_back.rend() - n, actual_back.rend(),
                          actual_back.rend() - where);
      REQUIRE(expected == actual_back);
    }

    // copy_backward_n
    {
      auto actual_back = arr;
      algo::copy_backward_n(actual_back.rend(), n, actual_back.rend() - where);
      REQUIRE(expected == actual_back);
    }
  };

  auto test_back = [](auto arr, int n, int where) {
    auto expected = arr;
    std::copy_backward(expected.begin(), expected.begin() + n,
                       expected.begin() + where + n);

    auto actual_fwd = arr;
    algo::copy(std::make_reverse_iterator(actual_fwd.rend()),
               std::make_reverse_iterator(actual_fwd.rend() - n),
               std::make_reverse_iterator(actual_fwd.rend() - where));

    auto actual_back = arr;
    algo::copy_backward(actual_back.begin(), actual_back.begin() + n,
                        actual_back.begin() + where + n);

    REQUIRE(expected == actual_back);
  };

  for (int n = 0; n < static_cast<int>(init.size()); ++n) {
    for (int where = 0; where < (static_cast<int>(init.size()) - n); ++where) {
      test_fwd(init, n, where);
      test_back(init, n, where);
    }
  }
}

TEST_CASE("algorithm.copy_n", "[algorithm]") {
  auto test = [](const std::vector<int>& ints) {
    const auto lints = algo::container_cast<std::list>(ints);

    // copy_n vector
    {
      const std::vector<int> expected = ints;
      std::vector<int> actual(ints.size());

      auto res = algo::copy_n(ints.begin(), ints.size(), actual.begin());

      REQUIRE(expected == actual);
      REQUIRE(res.first == ints.end());
      REQUIRE(res.second == actual.end());
    }

    // copy_n list
    {
      auto actual =
          algo::container_cast<std::list>(std::vector<int>(ints.size()));

      auto res = algo::copy_n(lints.begin(), lints.size(), actual.begin());

      REQUIRE(lints == actual);
      REQUIRE(res.first == lints.end());
      REQUIRE(res.second == actual.end());
    }

    // copy_backward_n vector
    {
      const std::vector<int> expected = ints;
      std::vector<int> actual(ints.size());

      auto res = algo::copy_backward_n(ints.end(), ints.size(), actual.end());

      REQUIRE(expected == actual);
      REQUIRE(res.first == ints.begin());
      REQUIRE(res.second == actual.begin());
    }

    // copy_backward_n list
    {
      auto actual =
          algo::container_cast<std::list>(std::vector<int>(ints.size()));

      auto res = algo::copy_backward_n(lints.end(), lints.size(), actual.end());

      REQUIRE(lints == actual);
      REQUIRE(res.first == lints.begin());
      REQUIRE(res.second == actual.begin());
    }
  };

  test({});
  test({1, 2, 3});
  test({1});
}

}  // namespace
}  // namespace algo
