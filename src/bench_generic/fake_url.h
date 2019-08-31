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

#ifndef BENCH_GENERIC_FAKE_URL_H
#define BENCH_GENERIC_FAKE_URL_H

#include <string>

namespace bench {

class fake_url {
  std::string data;

 public:
  fake_url() = default;

  explicit fake_url(int seed)
      : data("https://" + std::to_string(seed) + ".com") {}

  template <typename H>
  friend H AbslHashValue(H h, const fake_url& x) {
    return H::combine(std::move(h), x.data);
  }

  friend bool operator==(const fake_url& x, const fake_url& y) {
    return x.data == y.data;
  }

  friend bool operator<(const fake_url& x, const fake_url& y) {
    return x.data < y.data;
  }

  friend bool operator!=(const fake_url& x, const fake_url& y) {
    return !(x == y);
  }

  friend bool operator>(const fake_url& x, const fake_url& y) { return y < x; }

  friend bool operator<=(const fake_url& x, const fake_url& y) {
    return !(y < x);
  }

  friend bool operator>=(const fake_url& x, const fake_url& y) {
    return !(x < y);
  }
};

}  // namespace bench

#endif  // BENCH_GENERIC_FAKE_URL_H
