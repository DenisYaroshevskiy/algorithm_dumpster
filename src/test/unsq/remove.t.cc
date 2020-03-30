#include "unsq/remove.h"

#include <algorithm>
#include <numeric>

#include "test/catch.h"
#include "test/unsq/test_input.h"

namespace unsq {
namespace {

template <std::size_t width, typename I>
void one_range_remove_zero_test(I f, I l) {
  std::vector<ValueType<I>> expected, actual;

  auto run = [&] {
    expected = std::vector<ValueType<I>>(f, l);
    auto expected_end = std::remove(expected.begin(), expected.end(), 0);
    auto actual_end = unsq::remove<width>(f, l, 0);

    REQUIRE(expected_end - expected.begin() == actual_end - f);

    expected.erase(expected_end, expected.end());
    actual = std::vector<ValueType<I>>(f, actual_end);
  };

  // No zeroes
  std::iota(f, l, 1);
  run();
  REQUIRE(expected == actual);

  if (f == l) return;

  // Every even
  {
    std::iota(f, l, 1);
    I it = f;
    while(true) {
      if (it == l) break;
      *it++ = 0;
      if (it == l) break;
      ++it;
    }
    run();
    REQUIRE(expected == actual);
  }

  // Every odd
  {
    std::iota(f, l, 1);
    I it = ++f;  // returned if empty

    while(true) {
      if (it == l) break;
      *it++ = 0;
      if (it == l) break;
      ++it;
    }
    run();
    REQUIRE(expected == actual);
  }
}

TEST_CASE("remove very basic", "[simd]") {
  one_range_test([](auto f, auto l) {
    constexpr std::size_t small_pack_size = 16 / sizeof(ValueType<decltype(f)>);
    constexpr std::size_t big_pack_size = small_pack_size * 2;

    one_range_remove_zero_test<small_pack_size>(f, l);
    one_range_remove_zero_test<big_pack_size>(f, l);
  });
}

}  // namespace
}  // namespace unsq
