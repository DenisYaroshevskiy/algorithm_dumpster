/*
 * Copyright 2019 Denis Yaroshevskiy
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef BENCH_GENERIC_COUNTING_BENCHMARK_H
#define BENCH_GENERIC_COUNTING_BENCHMARK_H

#include <array>
#include <functional>
#include <ostream>
#include <string_view>
#include <vector>

namespace bench {
namespace detail {

struct counting_wrapper_base {
  inline static int copy;
  inline static int move;
  inline static int equal;
  inline static int less;
  inline static int hash;

  static auto tie_with_names() {
    using namespace std::literals;
    return std::array{std::pair{"copy"sv, &copy}, std::pair{"move"sv, &move},
                      std::pair{"equal"sv, &equal}, std::pair{"less"sv, &less},
                      std::pair{"hash"sv, &hash}};
  }

  static void clear() {
    for (auto& [name, ptr] : tie_with_names()) {
      (void)name;
      *ptr = 0;
    }
  }
};

}  // namespace detail

template <typename T>
struct counting_wrapper : detail::counting_wrapper_base {
  using base = detail::counting_wrapper_base;

  T body;

  counting_wrapper() = default;
  explicit counting_wrapper(T x) : body(std::move(x)) {}

  counting_wrapper(const counting_wrapper& x) : body(x.body) { ++base::copy; }
  counting_wrapper(counting_wrapper&& x) : body(std::move(x.body)) {
    ++base::move;
  }

  counting_wrapper& operator=(const counting_wrapper& x) {
    body = x.body;
    ++base::copy;
    return *this;
  }

  counting_wrapper& operator=(counting_wrapper&& x) {
    body = std::move(x.body);
    ++base::move;
    return *this;
  }

  template <typename H>
  friend H AbslHashValue(H h, const counting_wrapper& x) {
    ++base::hash;
    return H::combine(std::move(h), x.body);
  }

  friend bool operator==(const counting_wrapper& x, const counting_wrapper& y) {
    ++base::equal;
    return x.body == y.body;
  }

  friend bool operator!=(const counting_wrapper& x, const counting_wrapper& y) {
    return !(x == y);
  }

  friend bool operator<(const counting_wrapper& x, const counting_wrapper& y) {
    ++base::less;
    return x.body < y.body;
  }

  friend bool operator<=(const counting_wrapper& x, const counting_wrapper& y) {
    return !(y < x);
  }

  friend bool operator>(const counting_wrapper& x, const counting_wrapper& y) {
    return y < x;
  }

  friend bool operator>=(const counting_wrapper& x, const counting_wrapper& y) {
    return !(x < y);
  }
};

}  // namespace bench

namespace std {

template <typename T>
struct hash<bench::counting_wrapper<T>> {
  using argument_type = bench::counting_wrapper<T>;
  using result_type = typename std::hash<T>::result_type;

  result_type operator()(const argument_type& x) const {
    ++argument_type::hash;
    return std::hash<T>{}(x.body);
  }
};

}  // namespace std

namespace bench {

void clear_counters() {
    detail::counting_wrapper_base::clear();
}

void counters_to_json_dict(std::ostream& out, size_t tab_length = 0) {
  auto counters = detail::counting_wrapper_base::tie_with_names();

  std::string tab(tab_length, ' ');

  out << "{\n";

  auto f = counters.begin();

  for (; f != counters.end() - 1; ++f) {
    out << tab << "  \"" << f->first << "\": " << *f->second << ",\n";
  }

  out << tab << "  \"" << f->first << "\": " << *f->second << '\n'
      << tab << '}';
}

class counters_writer {
  std::ostream* out_;
  bool is_first_ = true;

 public:
  counters_writer(const counters_writer&) = default;
  counters_writer(counters_writer&&) = default;
  counters_writer& operator=(const counters_writer&) = default;
  counters_writer& operator=(counters_writer&&) = default;

  explicit counters_writer(std::ostream& out)
      : out_(&out) {
    *out_ << "{\n";
  }

  void operator()(std::string_view name) {
    if (!is_first_) *out_ << ",\n";
    is_first_ = false;
    *out_ << "  \"" << name << "\": ";
    counters_to_json_dict(*out_, 2);
  }

  ~counters_writer() {
    *out_ << "\n}";
  }
};

class counting_benchmark {
  std::vector<std::vector<int>> args_;
  counters_writer writer_;

 public:
   explicit counting_benchmark(std::ostream& out) : writer_(out) {}

   void args(std::vector<int> args) {
     args_.push_back(std::move(args));
   }

   template <typename Op>
   void run(std::string_view name, Op op) {
     for (const auto& cur  : args_) {
       std::string cur_name(name);
       for (int x : cur) {
         cur_name += '/' + std::to_string(x);
       }

       clear_counters();

       op(cur);
       writer_(cur_name);
     }
   }
};

}  // namespace bench

#endif  // BENCH_GENERIC_COUNTING_BENCHMARK_H
