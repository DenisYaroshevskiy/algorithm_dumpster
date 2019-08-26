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

#ifndef TEST_ZEROED_INT_H
#define TEST_ZEROED_INT_H

#include <algorithm>
#include <iostream>

namespace algo {

struct zeroed_int {
  int body = 0;

  zeroed_int() = default;
  explicit zeroed_int(int body) : body{body} {}
  zeroed_int(const zeroed_int&) = delete;
  zeroed_int(zeroed_int&& x) noexcept : body{std::exchange(x.body, 0)} {}
  zeroed_int& operator=(const zeroed_int&) = delete;
  zeroed_int& operator=(zeroed_int&& x) noexcept {
    body = std::exchange(x.body, 0);
    return *this;
  }
  ~zeroed_int() = default;

  friend bool operator==(const zeroed_int& x, const zeroed_int& y) {
    return x.body == y.body;
  }

  friend bool operator!=(const zeroed_int& x, const zeroed_int& y) {
    return !(x == y);
  }

  friend bool operator<(const zeroed_int& x, const zeroed_int& y) {
    return x.body < y.body;
  }

  friend bool operator>(const zeroed_int& x, const zeroed_int& y) {
    return y < x;
  }

  friend bool operator>=(const zeroed_int& x, const zeroed_int& y) {
    return !(x < y);
  }

  friend bool operator<=(const zeroed_int& x, const zeroed_int& y) {
    return !(y < x);
  }

  friend std::ostream& operator<<(std::ostream& out, const zeroed_int& x) {
    return out << x.body;
  }
};

}  // namespace algo

#endif  // TEST_ZEROED_INT_H
