"""
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
 *
"""

from datetime import date

import os
import itertools

# Comments ===============================================


def prefixComments():
    currentYear = int(date.today().year)

    license = \
        '''/*
 * Copyright {} Denis Yaroshevskiy
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
*/'''.format(currentYear)

    generatedWarning = \
        '''
/*
 * ================================================================
 *
 *  GENERATED CODE. Script: {}
 *
 * ================================================================
*/'''.format(__file__)

    return license + '\n' + generatedWarning


# Prefix/Suffix code =============================================

def prefixCode():
    return '''
#ifndef SIMD_MM_H_
#define SIMD_MM_H_

#include <immintrin.h>

#include <cstddef>
#include <cstdint>

namespace mm {

struct error_t { };

template <typename T>
struct type_t { using type = T; };
'''


def suffixCode():
    return '''

}  // namespace mm

# endif  // SIMD_MM_H_
'''

# Codegen utils ===================================================


def section(name):
    return '\n\n// ' + name + ' ' + '-' * (40 - len(name)) + '\n\n\n'


def privateNamespacePrefix():
    return 'namespace _mm {\n'


def privateNamespaceSuffix():
    return '}  // namepsace _mm\n'


def ifConstexprChainBasic(conditionBlock):
    res = ''

    for c, b in conditionBlock:
        res += 'if constexpr({}) '.format(c) + b + '\n else '

    res += '  return error_t{}; '

    return res


def makeConditionBlockPairs(condition, block, data):
    return [(condition.format(c), block.format(b)) for (c, b) in data]


def ifConstexprPattern(condition, block, data):
    return ifConstexprChainBasic(makeConditionBlockPairs(condition, block, data))


widthNamePairs = [(128, ''), (256, '256'), (512, '512')]


def instantiateJustRegister(template):
    return '\n'.join([template.format(size, name) for size, name in widthNamePairs])


def instantiateRegisterIntWidth(template, include64=False):
  intWidth = [8, 16, 32]
  if include64:
    intWidth.append(64)

  product = list(itertools.product(widthNamePairs, intWidth))
  flatten = list(itertools.chain(product))

  return '\n'.join(
    [template.format(rsize, name, tsize) for (rsize, name), tsize in flatten]
  )

# Actual work ================================================

# register ===================================

def register_i():
    res = privateNamespacePrefix()

    res += '''
template <std::size_t W>
constexpr auto register_i_impl() {
'''

    res += ifConstexprPattern(
        'W == {}', 'return type_t<{}>{{}};',
        [(128, '__m128i'), (256, '__m256i'), (512, '__m512i')]
    )

    res += '''
}
'''

    res += privateNamespaceSuffix()

    res += '''
template <std::size_t W>
using register_i = typename decltype(_mm::register_i_impl<W>())::type;
'''

    return res


# sizes =====================================================

def sizes():
    return '''
template <typename Register>
constexpr std::size_t bit_width() {
  if constexpr (std::is_same_v<Register, register_i<128>>) {
    return 128;
  } else if constexpr (std::is_same_v<Register, register_i<256>>) {
    return 256;
  } else if constexpr (std::is_same_v<Register, register_i<512>>) {
    return 512;
  }
  throw error_t{ };
}

template <typename Register>
constexpr size_t byte_width() {
  return bit_width<Register>() / 8;
}

template <typename Register>
constexpr size_t alignment() {
  return alignof(Register);
}
'''

# Load / Store ================================================


def load():
    pattern = '''
inline register_i<{0}> load(const register_i<{0}>* addr) {{
  return _mm{1}_load_si{0}(addr);
}}
'''
    return instantiateJustRegister(pattern)


def store():
    pattern = '''
inline void store(register_i<{0}>* addr, register_i<{0}> a) {{
  _mm{1}_store_si{0}(addr, a);
}}
'''
    return instantiateJustRegister(pattern)


# Set one value everywhere =======================================

def set0():
    res = '''
// Does not exist for floats.
template <size_t register_width>
inline auto setzero() {
'''

    pattern='''if constexpr (register_width == {0})
    return _mm{1}_setzero_si{0}();
  else'''

    res += instantiateJustRegister(pattern)

    res +='''
  return error_t{};
}
'''
    return res

def set1():
    res = '''
template <size_t register_width, typename T>
inline auto set1(T a) {
  static constexpr size_t t_width = sizeof(T) * 8;

'''

    pattern='''if constexpr (register_width == {0} && t_width == {2})
    return _mm{1}_set1_epi{2}(a);
  else'''

    res += instantiateRegisterIntWidth(pattern, include64=True)

    res +='''
  return error_t{};
}
'''
    return res

# Min/Max ===========================================

def min():
    res = '''
template <typename T, typename Register>
inline auto min(Register a, Register b) {
  static constexpr size_t register_width = bit_width<Register>();

'''
    pattern = '''if constexpr (register_width == {0}
    && std::is_same_v<T, std::int{2}_t>) return _mm{1}_min_epi{2}(a, b);
  else if constexpr (register_width == {0}
    && std::is_same_v<T, std::uint{2}_t>) return _mm{1}_min_epu{2}(a, b);
  else'''

    res += instantiateRegisterIntWidth(pattern, include64=True)

    res +='''
  return error_t{};
}
'''

    return res

def max():
    res = '''
template <typename T, typename Register>
inline auto max(Register a, Register b) {
  static constexpr size_t register_width = bit_width<Register>();

'''
    pattern = '''if constexpr (register_width == {0}
    && std::is_same_v<T, std::int{2}_t>) return _mm{1}_max_epi{2}(a, b);
  else if constexpr (register_width == {0}
    && std::is_same_v<T, std::uint{2}_t>) return _mm{1}_max_epu{2}(a, b);
  else'''

    res += instantiateRegisterIntWidth(pattern, include64=True)

    res +='''
  return error_t{};
}
'''

    return res

def generateMainCode():
    res = ''
    res += section('register_i')
    res += register_i()

    res += section('sizes')
    res += sizes()

    res += section('load/store')
    res += load()
    res += store()

    res += section('set one value everywhere')
    res += set0()
    res += set1()

    res += section('min/max')
    res += min()
    res += max()

    return res


# Fixups ================================

def fixUp(res):
  return res.replace('set1_epi64', 'set1_epi64x')

# Driver ==================================

def generateCode():
    res = prefixComments()
    res += prefixCode()
    res += generateMainCode()
    res += suffixCode()

    res = fixUp(res)
    return res


if __name__ == '__main__':
    mm_h = os.path.dirname(os.path.abspath(__file__)) + '/mm.h'

    f = open(mm_h, "w")
    f.write(generateCode())
    f.close()

    os.system('clang-format-mp-7.0 -i ' + mm_h)
