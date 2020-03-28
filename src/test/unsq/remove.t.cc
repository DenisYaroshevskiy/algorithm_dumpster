#include "unsq/remove.h"

#include "test/catch.h"

namespace unsq {
namespace {

TEST_CASE("remove very basic", "[simd]") {
  std::vector<std::int16_t> data = {1, 2, 0, 3, 4, 5, 0, 6, 7, 8, 9, 0};

  data.erase(
    unsq::remove<8>(data.begin(), data.end(), 0),
    data.end()
  );

  REQUIRE(data == std::vector<std::int16_t>{1, 2, 3, 4, 5, 6, 7, 8, 9});
}

}  // namespace
}  // namespace unsq
