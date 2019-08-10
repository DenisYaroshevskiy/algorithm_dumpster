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
