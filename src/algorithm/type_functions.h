#ifndef ALGORITHM_TYPE_FUNCTIONS_H
#define ALGORITHM_TYPE_FUNCTIONS_H

#include <iterator>
#include <type_traits>

namespace tools {

template <typename I>
using ValueType = typename std::iterator_traits<I>::value_type;

template <typename I>
using DifferenceType = typename std::iterator_traits<I>::difference_type;

template <typename I>
using Pointer = typename std::iterator_traits<I>::pointer;

template <typename I>
using Reference = typename std::iterator_traits<I>::reference;

template <typename I>
using IteratorCategory = typename std::iterator_traits<I>::iterator_category;

template <typename I>
constexpr bool RandomAccessIterator =
    std::is_base_of_v<std::random_access_iterator_tag, IteratorCategory<I>>;

}  // namespace tools

#endif  // ALGORITHM_TYPE_FUNCTIONS_H
