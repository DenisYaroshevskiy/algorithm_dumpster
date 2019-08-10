#ifndef ALGORITHM_NTH_PERMUTATION_H
#define ALGORITHM_NTH_PERMUTATION_H

#include <cassert>
#include <utility>
#include <vector>

#include "algorithm/factoriadic_representation.h"
#include "algorithm/find_nth.h"
#include "algorithm/make_vector_of_iterators.h"

namespace algo {

template <typename I, typename O, typename N>
// requires ForwardIterator<I> && OutputIterator<0> && Number<N>
O nth_permutation(I f, I l, O o, N n) {
  if (f == l) {
    assert(!n);
    return o;
  }

  std::vector<I> markers = make_vector_of_iterators(f, l);
  std::vector<DifferenceType<I>> factoriadic_n(markers.size());
  to_factoriadic_representation(std::move(n), factoriadic_n.rbegin());

  for (DifferenceType<I> pos : factoriadic_n) {
    // TODO: we can potentially reduce the number of searches by updating
    //       the left boundary.
    // TODO: Using 'colony' data structure could reduce the cost of the
    //       algorithm: https://youtu.be/wBER1R8YyGY
    auto marker_it = find_nth_if_guarantied(markers.begin(), pos,
                                            [&](I x) { return x != l; });
    *o++ = **marker_it;
    *marker_it = l;
  }

  return o;
}

}  // namespace algo

#endif  // ALGORITHM_NTH_PERMUTATION_H