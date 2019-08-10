#ifndef ALGORITHM_FACTORIAL_H
#define ALGORITHM_FACTORIAL_H

namespace tools {

template <typename ResultN, typename N>
constexpr ResultN factorial(N n) {
  ResultN res{1};

  for (N i = 2; i <= n; ++i) {
    res *= i;
  }

  return res;
}

}  // namespace tools

#endif  // ALGORITHM_FACTORIAL_H