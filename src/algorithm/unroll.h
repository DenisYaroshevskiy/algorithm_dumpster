#ifndef ALGORITHM_UNROLL_H
#define ALGORITHM_UNROLL_H

#include <cstddef>
#include <utility>

namespace algo {
namespace detail {

template <typename Op, size_t... from0_to_n>
constexpr void unroll_impl(Op op, std::index_sequence<from0_to_n...>) {
  (((void)from0_to_n, op()), ...);
}

}  // namespace detail

template <size_t N, typename Op>
// require Callable<Op>
constexpr void unroll(Op op) {
  detail::unroll_impl(op, std::make_index_sequence<N>{});
}

}  // namespace algo

#endif  // ALGORITHM_UNROLL_H