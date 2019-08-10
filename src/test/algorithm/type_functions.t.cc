#include "algorithm/type_functions.h"

#include <list>

#include "test/catch.h"

namespace algo {
namespace {

TEST_CASE("iterator categories", "[aglorithm]") {
  static_assert(RandomAccessIterator<int*>);
  static_assert(!RandomAccessIterator<std::list<int>::iterator>);
}

}  // namespace
}  // namespace algo
