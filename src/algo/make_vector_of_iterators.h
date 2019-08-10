#ifndef ALGO_MAKE_VECTOR_OF_ITERATORS_H
#define ALGO_MAKE_VECTOR_OF_ITERATORS_H

#include <algorithm>
#include <numeric>
#include <vector>

#include "algo/type_functions.h"

namespace algo {

template <typename I>
// require ForwardIterator<I>
std::vector<I> make_vector_of_iterators(I f, I l) {
  std::vector<I> res;
  if constexpr (RandomAccessIterator<I>) {
    res.resize(std::distance(f, l));
    std::iota(res.begin(), res.end(), f);
  } else {
    while (f != l) {
      res.push_back(f);
      ++f;
    }
  }
  return res;
}

}  // namespace algo

#endif  // ALGO_MAKE_VECTOR_OF_ITERATORS_H
