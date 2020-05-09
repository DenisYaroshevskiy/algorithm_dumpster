#ifndef UNSQ_REDUCE_H_
#define UNSQ_REDUCE_H_

#include <optional>

#include "simd/pack.h"
#include "unsq/drill_down.h"
#include "unsq/iteration.h"

namespace unsq {

template <std::size_t width, typename I, typename T, typename Op>
// require ContigiousIterator<I> &&
//         CommutaiveAssociativeTransformation<Op,
//         pack<equivalent<ValueType<I>>, width>> && same<equivalent<T>,
//         equivalent<ValueType<I>>
ValueType<I> reduce(I f, I l, const T& zero, Op op) {
  using pack = simd::pack<equivalent<ValueType<I>>, width>;
  const auto zeroes = simd::set_all<pack>(equivalent_cast(zero));

  auto res = zeroes;

  auto body = [&](equivalent_iterator<I> from, auto... ignore) mutable {
    auto cur = simd::load<pack>(from);
    cur = simd::replace_ignored(cur, ignore..., zeroes);
    res = op(res, cur);
    return false;
  };

  iteration_aligned<width>(f, l, body);

  res = simd::reduce(res, op);
  auto as_array = simd::to_array(res);
  return from_equivalent_cast<ValueType<I>>(as_array[0]);
}

template <std::size_t width, typename I, typename T>
ValueType<I> reduce(I f, I l, const T& zero) {
  return reduce<width>(
      f, l, zero, [](auto xs, auto ys) { return simd::add_pairwise(xs, ys); });
}

template <std::size_t width, typename I>
ValueType<I> reduce(I f, I l) {
  return reduce<width>(f, l, ValueType<I>{});
}

template <std::size_t width, typename I>
std::optional<ValueType<I>> min_value(I f, I l) {
  if (f == l) return std::nullopt;

  // First element is a valid zero for a min_pairwise.
  return reduce<width>(
      f, l, *f, [](auto xs, auto ys) { return simd::min_pairwise(xs, ys); });
}

template <std::size_t width, typename I>
std::optional<ValueType<I>> max_value(I f, I l) {
  if (f == l) return std::nullopt;

  // First element is a valid zero for a max_pairwise.
  return reduce<width>(
      f, l, *f, [](auto xs, auto ys) { return simd::max_pairwise(xs, ys); });
}

}  // namespace unsq

#endif  // UNSQ_REDUCE_H_
