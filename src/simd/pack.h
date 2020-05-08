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

#ifndef SIMD_PACK_H_
#define SIMD_PACK_H_

#include "simd/pack_detail/comparisons.h"
#include "simd/pack_detail/comparisons_pairwise.h"
#include "simd/pack_detail/minmax_pairwise.h"

#include "simd/pack_detail/load.h"
#include "simd/pack_detail/store.h"
#include "simd/pack_detail/set.h"

#include "simd/pack_detail/blend.h"

#include "simd/pack_detail/arithmetic_pairwise.h"

#include "simd/pack_detail/bit_operations.h"

#include "simd/pack_detail/operators.h"
#include "simd/pack_detail/pack_declaration.h"

#include "simd/pack_detail/address_manipulation.h"
#include "simd/pack_detail/top_bits.h"
#include "simd/pack_detail/spread_top_bits.h"

#include "simd/pack_detail/compress_mask.h"
#include "simd/pack_detail/compress.h"

#include "simd/pack_detail/shuffle.h"

#include "simd/pack_detail/reduce.h"

#endif  // SIMD_PACK_H_
