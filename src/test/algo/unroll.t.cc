#include "algo/unroll.h"

#include "test/catch.h"

namespace algo {
namespace {

template <size_t N>
constexpr int add_n() {
  int r = 0;
  unroll<N>([&r]() mutable { ++r; });
  return r;
}

TEST_CASE("algorithm.unroll", "[algorithm]") {
  static_assert(add_n<0>() == 0);
  static_assert(add_n<1>() == 1);
  static_assert(add_n<5>() == 5);
}

}  // namespace
}  // namespace algo
