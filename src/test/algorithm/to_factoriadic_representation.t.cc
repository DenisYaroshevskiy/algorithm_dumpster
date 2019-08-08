#include "algorithm/to_factoriadic_representation.h"

#include <algorithm>
#include <array>
#include <numeric>

#include "test/catch.h"

namespace tools {
namespace {

struct FailCompilation {};

template <size_t N>
constexpr bool test_factoriadic(int value, std::array<int, N> expected) {
  if (compute_factoriadic_representation_length(value) != N) {
    throw FailCompilation{};
  }
  std::array<int, N> actual = {};
  to_factoriadic_representation(value, actual.begin());

  // non-constexpr array still
  for (size_t i = 0; i < N; ++i) {
    if (actual[i] != expected[i]) return false;
  }
  return true;
}

TEST_CASE("algorithm.to_factoriadic_representation builtin", "[algorithm]") {
  // Specific examples
  {
    static_assert(test_factoriadic(0, std::array{0}));
    static_assert(test_factoriadic(1, std::array{0, 1}));

    static_assert(test_factoriadic(2, std::array{0, 0, 1}));
    static_assert(test_factoriadic(3, std::array{0, 1, 1}));
    static_assert(test_factoriadic(4, std::array{0, 0, 2}));
    static_assert(test_factoriadic(5, std::array{0, 1, 2}));

    static_assert(test_factoriadic(6, std::array{0, 0, 0, 1}));

    static_assert(test_factoriadic(349, std::array{0, 1, 0, 2, 4, 2}));
  }

  {
    for (int i = 0; i < 1000; ++i) {
      std::vector<int> representation(
          compute_factoriadic_representation_length(i));
      to_factoriadic_representation(i, representation.begin());

      int next_multiple = 1;
      int factorial = 1;
      int actual =
          std::accumulate(representation.begin() + 1, representation.end(), 0,
                          [&](int sum, int digit) {
                            factorial *= next_multiple++;
                            return sum + factorial * digit;
                          });
      REQUIRE(i == actual);
    }
  }
}

}  // namespace
}  // namespace tools