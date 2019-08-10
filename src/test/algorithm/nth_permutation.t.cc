#include "algorithm/nth_permutation.h"

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

#include <boost/multiprecision/cpp_int.hpp>

#include "test/catch.h"

namespace tools {
namespace {

using big_int = boost::multiprecision::cpp_int;

TEST_CASE("algorithm.nth_permutation.5ints", "[algorithm]") {
  std::vector<int> sorted(5);
  std::iota(sorted.begin(), sorted.end(), 0);

  std::vector<int> actual = sorted;

  for (std::int64_t i = 0;; ++i) {
    std::vector<int> expected(sorted.size());

    INFO("permutation number: " << i);
    nth_permutation(sorted.begin(), sorted.end(), expected.begin(), i);

    REQUIRE(expected == actual);
    if (!std::next_permutation(actual.begin(), actual.end())) break;
  }
}

TEST_CASE("algorithm.nth_permutation.special_cases", "[algorithm]") {
  {
    std::vector<int> v1, v2;
    nth_permutation(v1.begin(), v1.end(), v2.begin(), 0);
  }
  {
    std::vector<int> v1{1}, v2{1};
    nth_permutation(v1.begin(), v1.end(), v2.begin(), 0);
  }
  {
    static constexpr size_t size = 1000;
    std::vector<int> sorted(size), expected(size), actual(size);

    std::iota(sorted.begin(), sorted.end(), 0);
    std::reverse_copy(sorted.begin(), sorted.end(), expected.begin());

    big_int last_permuation_number = [] {
      big_int res{1};

      for (int i = 2; i <= static_cast<int>(size); ++i) {
        res *= i;
      }

      res -= 1;
      return res;
    }();

    nth_permutation(sorted.begin(), sorted.end(), actual.begin(),
                    std::move(last_permuation_number));
    REQUIRE(expected == actual);
  }
}

}  // namespace
}  // namespace tools