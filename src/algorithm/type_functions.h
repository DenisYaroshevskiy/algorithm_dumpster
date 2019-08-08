#ifndef ALGORITHM_TYPE_FUNCTIONS_H
#define ALGORITHM_TYPE_FUNCTIONS_H

#include <iterator>

namespace tools {

template <typename I>
using ValueType = typename std::iterator_traits<I>::value_type;

template <typename I>
using DifferenceType = typename std::iterator_traits<I>::difference_type;

}  // namespace tools

#endif  // ALGORITHM_TYPE_FUNCTIONS_H
