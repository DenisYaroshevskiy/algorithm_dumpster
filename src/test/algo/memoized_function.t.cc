#include "algo/memoized_function.h"

#include "test/catch.h"

namespace algo {
namespace {

TEST_CASE("algorithm.memoized_function", "[algorithm]") {
  int value = 0;
  auto op = memoized_function<int>([&](int) mutable { return value; });

  REQUIRE(op(0) == 0);
  ++value;
  REQUIRE(op(0) == 0);
  REQUIRE(op(1) == 1);
}

}  // namespace
}  // namespace algo
