#include "algorithm/nth_permutation.h"

#include <algorithm>
#include <cstdint>
#include <numeric>
#include <vector>

#include "test/catch.h"

namespace tools {
namespace {

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
}

}  // namespace
}  // namespace tools