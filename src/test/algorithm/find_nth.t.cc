#include "algorithm/find_nth.h"

#include <array>

#include "test/catch.h"

namespace tools {
namespace {

TEST_CASE("algorithm.find_nth_guarantied", "[algorithm]") {
  std::array data{1, 1, 1, 1, 1, 1};

  for (int i = 0; i < static_cast<int>(data.size()); ++i) {
      int* found = find_nth_guarantied(data.begin(), i, 1);
      REQUIRE(found - data.begin() == i);
  }
}

}  // namespace
}  // namespace tools