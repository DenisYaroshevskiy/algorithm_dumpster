#ifndef ALGO_FACTORIAL_H
#define ALGO_FACTORIAL_H

namespace algo {

template <typename ResultN, typename N>
// require Integral<ResultN> && Integral<N>
constexpr ResultN factorial(N n) {
  ResultN res{1};

  for (N i = 2; i <= n; ++i) {
    res *= i;
  }

  return res;
}

}  // namespace algo

#endif  // ALGO_FACTORIAL_H
