#include "bench_generic/sort.h"

#include <algorithm>

#include "bench_generic/set_parameters.h"

namespace bench {

struct std_sort {
  template <typename I, typename Cmp>
  void operator()(I f, I l, Cmp cmp) {
    std::sort(f, l, cmp);
  }
};

struct std_stable_sort {
  template <typename I, typename Cmp>
  void operator()(I f, I l, Cmp cmp) {
    std::stable_sort(f, l, cmp);
  }
};

#if defined(STD_SORT)
using alg = std_sort;
#elif defined(STD_STABLE_SORT)
using alg = std_stable_sort;
#endif

BENCHMARK_TEMPLATE(sort_int_vec, 1000, alg)->Apply(set_every_5th_percent);

}  // namespace bench
