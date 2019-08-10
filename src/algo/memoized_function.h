#ifndef ALGO_MEMOIZED_FUNCTION_H
#define ALGO_MEMOIZED_FUNCTION_H

#include <map>
#include <type_traits>

namespace algo {

template <typename T, typename Op>
// require TotallyOrdered<T> && UnaryFunction<Op, T>
auto memoized_function(Op op) {
  using result_type = decltype(op(std::declval<T>()));
  return [cache = std::map<T, result_type>{}, op](const T& x) mutable {
    auto lb = cache.lower_bound(x);
    if (lb != cache.end() && lb->first == x) return lb->second;
    return cache.insert(lb, {x, op(x)})->second;
  };
}

}  // namespace algo

#endif  // ALGO_MEMOIZED_FUNCTION_H
