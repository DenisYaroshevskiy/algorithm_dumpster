#include "bench_generic/input_generators.h"

#include <array>

#include "test/catch.h"

namespace bench {
namespace detail {
namespace {

TEST_CASE("bench.input_generators.generate_sorted_vector", "[bench]") {
  std::array inputs = {0, 5, 2, 2, 1};
  auto src = [&, pos = 0]() mutable { return inputs[pos++]; };

  auto res = generate_sorted_vector<int>(inputs.size(), src);
  REQUIRE(res == std::vector{0, 1, 2, 2, 5});
}

TEST_CASE("bench.input_generators.generate_unique_sorted_vector", "[bench]") {
  std::array inputs = {0, 5, 2, 2, 1};
  auto src = [&, pos = 0]() mutable { return inputs[pos++]; };

  auto res = generate_unique_sorted_vector<int>(inputs.size() - 1, src);
  REQUIRE(res == std::vector{0, 1, 2, 5});
}

}  // namespace
}  // namespace detail
}  // namespace bench