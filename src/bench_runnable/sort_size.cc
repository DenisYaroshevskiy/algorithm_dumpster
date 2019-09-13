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

#include "bench_generic/sort.h"

#include "bench_generic/sort_function_objects.h"
#include "bench_generic/set_parameters.h"

namespace bench {

BENCHMARK_TEMPLATE(sort_vec_size, SELECTED_ALGORITHM, SELECTED_TYPE)
    ->Apply(set_5_size_increases<SELECTED_NUMBER, 10>);

}  // namespace bench