#include "algo/factoriadic_representation.h"

#include <algorithm>
#include <array>
#include <numeric>

#include <boost/multiprecision/cpp_int.hpp>

#include "test/catch.h"

namespace algo {
namespace {

using big_int = boost::multiprecision::cpp_int;

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

  return value ==
         from_factoriadic_representation<int>(expected.begin(), expected.end());
}

template <typename N>
void test_factoriadic_runtime(N n) {
  std::vector<int> representation(compute_factoriadic_representation_length(n));
  to_factoriadic_representation(n, representation.begin());
  N actual = from_factoriadic_representation<N>(representation.begin(),
                                                representation.end());
  REQUIRE(n == actual);
}

TEST_CASE("algorithm.to_from_factoriadic_representation.builtin",
          "[algorithm]") {
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
      test_factoriadic_runtime(i);
    }
  }
}

TEST_CASE("algorithm.to_factoriadic_representation.cpp_int", "[algorithm]") {
  {
    big_int x{6};
    std::ptrdiff_t lenght_default =
        compute_factoriadic_representation_length(x);
    int length_int = compute_factoriadic_representation_length<int>(x);
    short length_short =
        compute_factoriadic_representation_length<short, int>(x);
    REQUIRE(lenght_default == 4);
    REQUIRE(length_int == 4);
    REQUIRE(length_short == 4);
  }
  {
    auto factorial = [](int x) {
      big_int res(1);
      for (int i = 1; i < x; ++i) {
        res *= i;
      }
      return res;
    };

    for (int i = 100; i < 200; ++i) test_factoriadic_runtime(factorial(i));
  }
}

}  // namespace
}  // namespace algo
