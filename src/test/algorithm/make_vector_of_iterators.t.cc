#include "algorithm/make_vector_of_iterators.h"

#include <list>
#include <vector>

#include "test/catch.h"

namespace tools {
namespace {

TEST_CASE("algorithm.make_vector_of_iterators", "[algorithm]") {
  auto run_test = [](auto f, auto l) {
    auto actual = make_vector_of_iterators(f, l);
    auto res_f = actual.begin();

    while (f != l) {
      REQUIRE(f == *res_f);
      ++f;
      ++res_f;
    }

    REQUIRE(res_f == actual.end());
  };

  {
    std::vector<int> v;
    run_test(v.begin(), v.end());

    v.resize(5);
    run_test(v.begin(), v.end());
    run_test(&v[0], &v[0] + v.size());
  }

  {
    std::list<int> l;
    run_test(l.begin(), l.end());

    for (int i = 0; i < 5; ++i) {
      l.push_back(i);
    }
    run_test(l.begin(), l.end());
  }
}

}  // namespace
}  // namespace tools