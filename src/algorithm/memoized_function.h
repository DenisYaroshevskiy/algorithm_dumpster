#ifndef ALGORITHM_MEMOIZED_FUNCTION_H
#define ALGORITHM_MEMOIZED_FUNCTION_H

#include <map>
#include <type_traits>

namespace tools {

template <typename T, typename Op>
auto memoized_function(Op op) {
  using result_type = decltype(op(std::declval<T>()));
  return [cache = std::map<T, result_type>{}, op](const T& x) mutable {
      auto lb = cache.lower_bound(x);
      if (lb != cache.end() && lb->first == x) return lb->second;
      return cache.insert(lb, {x, op(x)})->second;
  };
}

}  // namespace tools

#endif  // ALGORITHM_MEMOIZED_FUNCTION_H