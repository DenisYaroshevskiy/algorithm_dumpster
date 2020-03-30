/*
 * Copyright 2020 Denis Yaroshevskiy
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

#ifndef TEST_UNSQ_TEST_INPUT_H_
#define TEST_UNSQ_TEST_INPUT_H_

#include <memory>

#include <stdexcept>
#include <string>

#include "simd/pack.h"

namespace unsq {

constexpr auto page_alignment = std::align_val_t{simd::page_size()};

struct page_deallocator {
  template <typename T>
  void operator()(T* ptr) {
    ::operator delete[](ptr, page_alignment);
  }
};

template <typename T>
class page_boundary_ranges {
 public:
  page_boundary_ranges();

  const std::vector<std::pair<T*, T*>>& ranges() const { return ranges_; }

 private:
  static constexpr std::size_t n_in_page = simd::page_size() / sizeof(T);

  std::vector<std::pair<T*, T*>> ranges_;
  std::unique_ptr<T, page_deallocator> one_page_;
  std::unique_ptr<T, page_deallocator> two_pages_;
  std::unique_ptr<T, page_deallocator> another_two_pages_;

  void assert_last_range_is_valid(std::ptrdiff_t f_plus,
                                  std::ptrdiff_t l_plus) const {
    T* f = ranges_.back().first;
    T* l = ranges_.back().second;

    auto within_page = [](T* start, T* ptr) {
      return start <= ptr && ptr <= start + n_in_page;
    };

    auto within_2_pages = [](T* start, T* ptr) {
      return start <= ptr && ptr <= start + 2 * n_in_page;
    };

    auto check = [&] {
      if (l < f) return false;

      if (within_page(one_page_.get(), f) && within_page(one_page_.get(), l))
        return true;
      if (within_2_pages(two_pages_.get(), f) &&
          within_2_pages(two_pages_.get(), l))
        return true;
      return within_2_pages(another_two_pages_.get(), f) &&
             within_2_pages(another_two_pages_.get(), l);
    };

    if (check()) return;

    throw std::runtime_error("invalid range: f_plus " + std::to_string(f_plus) +
                             ", l_plus: " + std::to_string(l_plus));
  }
};

template <typename T>
page_boundary_ranges<T>::page_boundary_ranges() {
  one_page_.reset(new (page_alignment) T[n_in_page]);
  two_pages_.reset(new (page_alignment) T[2 * n_in_page]);
  another_two_pages_.reset(new (page_alignment) T[2 * n_in_page]);

  T* op = one_page_.get();
  T* tp = two_pages_.get();
  T* sp = tp + n_in_page;

  T* atp = another_two_pages_.get();
  T* asp = atp + n_in_page;

  auto add_range = [&](T* base, std::ptrdiff_t f_plus, std::ptrdiff_t l_plus) {
    T* f = base + (f_plus / sizeof(T));
    T* l = base + (l_plus / sizeof(T));
    ranges_.push_back({f, l});
    assert_last_range_is_valid(f_plus, l_plus);
  };

  // From the beginning, 0 <= size < 16
  add_range(op, 0, 0);
  add_range(sp, 0, 0);
  add_range(op, 0, 10);
  add_range(tp, 0, 15);
  add_range(sp, 0, 3);

  // From the beginning, 16 <= x < 32
  add_range(op, 0, 16);
  add_range(tp, 0, 16);
  add_range(op, 0, 20);
  add_range(tp, 0, 23);
  add_range(sp, 0, 31);

  // From the beginning, 32 <= x < page_size
  add_range(op, 0, 32);
  add_range(tp, 0, 32);
  add_range(op, 0, 63);
  add_range(tp, 0, 71);
  add_range(sp, 0, 256);
  add_range(op, 0, 870);
  add_range(tp, 0, 3000);
  add_range(sp, 0, 4000);

  // From the beginning, page_size <= x <= 2 * page_size
  add_range(op, 0, simd::page_size());
  add_range(tp, 0, simd::page_size());
  add_range(sp, 0, simd::page_size());
  add_range(tp, 0, 2 * simd::page_size());
  add_range(tp, 0, 3 * simd::page_size() / 2);
  add_range(tp, 0, simd::page_size() + 1000);
  add_range(atp, 0, 2 * simd::page_size());
  add_range(atp, 0, simd::page_size() + 2348);
  add_range(atp, 0, simd::page_size() + 1200);

  // Middle of the page, 0 <= size < 16
  add_range(op, 31, 31);
  add_range(tp, 1000, 1000);
  add_range(op, 1040, 1051);
  add_range(tp, 2010, 2018);
  add_range(sp, 1000, 1111);

  // Middle of the page, 16 <= size < 32
  add_range(op, 32, 32 + 16);
  add_range(tp, 64, 64 + 16);
  add_range(op, 1071, 1071 + 18);
  add_range(tp, 2020, 2020 + 19);
  add_range(sp, 800, 800 + 30);

  // Middle of the page, 32 <= x < page_size
  add_range(op, 128, 128 + 32);
  add_range(tp, 256, 256 + 32);
  add_range(op, 100, 2000);
  add_range(tp, 50, 2080);
  add_range(sp, -20, 1000);
  add_range(asp, -16, 16);
  add_range(asp, -100, 400);
  add_range(sp, -300, 1800);

  // Middle of the page, page_size <= x <= 2 * page_size
  add_range(tp, 2048, 2048 + simd::page_size());
  add_range(atp, 1024 * 3, 1024 * 3 + simd::page_size());
  add_range(sp, -500, simd::page_size() - 400);
  add_range(sp, -1120, simd::page_size() - 1000);
  add_range(asp, -1000, simd::page_size() - 800);
  add_range(asp, -3020, simd::page_size() - 2030);

  // End of page, 0 <= size < 16
  add_range(op, simd::page_size(), simd::page_size());
  add_range(sp, simd::page_size(), simd::page_size());
  add_range(op, simd::page_size() - 10, simd::page_size());
  add_range(sp, simd::page_size() - 8, simd::page_size());
  add_range(asp, simd::page_size() - 15, simd::page_size());
  add_range(asp, simd::page_size() - 11, simd::page_size());

  // End of page, 16 <= size < 32
  add_range(op, simd::page_size() - 16, simd::page_size());
  add_range(sp, simd::page_size() - 16, simd::page_size());
  add_range(op, simd::page_size() - 31, simd::page_size());
  add_range(sp, simd::page_size() - 23, simd::page_size());
  add_range(asp, simd::page_size() - 18, simd::page_size());

  // End of page, 32 <= x < page_size
  add_range(op, simd::page_size() - 32, simd::page_size());
  add_range(sp, simd::page_size() - 32, simd::page_size());
  add_range(op, simd::page_size() - 1024, simd::page_size());
  add_range(sp, simd::page_size() - 68, simd::page_size());
  add_range(asp, simd::page_size() - 2020, simd::page_size());
  add_range(op, simd::page_size() - 3072, simd::page_size());
  add_range(sp, simd::page_size() - 4000, simd::page_size());
  add_range(asp, simd::page_size() - 2012, simd::page_size());

  // End of page, page_size <= x <= 2 * page_size
  add_range(sp, -1000, simd::page_size());
  add_range(asp, -1290, simd::page_size());
  add_range(sp, -1, simd::page_size());
  add_range(asp, -14, simd::page_size());
}

template <typename T, typename Op>
void one_range_test_one_type(Op op) {
  page_boundary_ranges<T> pbr;

  for (auto r : pbr.ranges()) op(r.first, r.second);
}

template <typename Op>
void one_range_test(Op op) {
  one_range_test_one_type<std::int8_t>(op);
  one_range_test_one_type<std::uint8_t>(op);
  one_range_test_one_type<std::int16_t>(op);
  one_range_test_one_type<std::uint16_t>(op);
  one_range_test_one_type<std::int32_t>(op);
  one_range_test_one_type<std::uint32_t>(op);
  one_range_test_one_type<std::int64_t>(op);
  one_range_test_one_type<std::uint64_t>(op);
}

}  // namespace unsq

#endif  // TEST_UNSQ_TEST_INPUT_H_
