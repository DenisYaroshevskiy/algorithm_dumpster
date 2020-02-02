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
#include <type_traits>

namespace mm {

struct error_t { };

template <typename T>
struct type_t { using type = T; };

// Helper to support pointers and more isoteric things like std::uintptr_t.
template <typename T, typename Int>
constexpr bool is_equivalent() {
  if (sizeof(T) != sizeof(Int)) return false;
  if (std::is_signed_v<Int>) return std::is_signed_v<T> || std::is_pointer_v<T>;
  return !std::is_signed_v<T>;
}
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


def instantiateRegisterIntWidth(template, include64=True):
    intWidth = [8, 16, 32]
    if include64:
        intWidth.append(64)

    product = list(itertools.product(widthNamePairs, intWidth))
    flatten = list(itertools.chain(product))

    return '\n'.join(
        [template.format(rsize, name, tsize)
         for (rsize, name), tsize in flatten]
    )


def instantiateIfConstexprPattern_justRegister(condition, action):
    pattern = 'if constexpr (' + condition + ')' + action + 'else'
    return instantiateJustRegister(pattern) + '  return error_t{}; }\n'


def instantiateIfConstexprPattern_intWidth(condition, action):
    pattern = 'if constexpr (' + condition + ')' + action + 'else'
    return instantiateRegisterIntWidth(pattern) + '  return error_t{}; }\n'


def instantiateIfConstexprPattern_intWidth_twice(condition1, action1, condition2, action2):
    pattern = 'if constexpr (' + condition1 + ')' + action1 + 'else '
    pattern += 'if constexpr (' + condition2 + ')' + action2 + 'else'
    return instantiateRegisterIntWidth(pattern) + '  return error_t{}; }\n'


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


def load_ignore_asan():
    pattern = '''
__attribute__((no_sanitize_address))
inline register_i<{0}> load_ignore_asan(const register_i<{0}>* addr) {{
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
template <typename Register>
inline auto setzero() {
  static constexpr size_t register_width = bit_width<Register>();
'''
    return res + instantiateIfConstexprPattern_justRegister(
        'register_width == {0}',
        'return _mm{1}_setzero_si{0}();')


def set1():
    res = '''
template <typename Register, typename T>
inline auto set1(T a) {
  static constexpr std::size_t register_width = bit_width<Register>();
  static constexpr std::size_t t_width = sizeof(T) * 8;

'''

    return res + instantiateIfConstexprPattern_intWidth(
        'register_width == {0} && t_width == {2}',
        'return _mm{1}_set1_epi{2}(a);'
    )

# Min/Max ===========================================


def min():
    res = '''
template <typename T, typename Register>
inline auto min(Register a, Register b) {
  static constexpr size_t register_width = bit_width<Register>();

'''

    return res + instantiateIfConstexprPattern_intWidth_twice(
        'register_width == {0} && is_equivalent<T, std::int{2}_t>()',
        'return _mm{1}_min_epi{2}(a, b);',
        'register_width == {0} && is_equivalent<T, std::uint{2}_t>()',
        'return _mm{1}_min_epu{2}(a, b);'
    )


def max():
    res = '''
template <typename T, typename Register>
inline auto max(Register a, Register b) {
  static constexpr size_t register_width = bit_width<Register>();

'''

    return res + instantiateIfConstexprPattern_intWidth_twice(
        'register_width == {0} && is_equivalent<T, std::int{2}_t>()',
        'return _mm{1}_max_epi{2}(a, b);',
        'register_width == {0} && is_equivalent<T, std::uint{2}_t>()',
        'return _mm{1}_max_epu{2}(a, b);'
    )

# Comparisons ==========================================


def cmpeq():
    res = '''
  template <typename T, typename Register>
  inline auto cmpeq(Register a, Register b) {
    static constexpr size_t register_width = bit_width<Register>();
    static constexpr size_t t_width = sizeof(T) * 8;
'''

    return res + instantiateIfConstexprPattern_intWidth(
        'register_width == {0} && t_width == {2}',
        'return _mm{1}_cmpeq_epi{2}(a, b);'
    )

def cmpgt():
    res = '''
  // Instruction is not avaliable for unsigned ints.
  template <typename T, typename Register>
  inline auto cmpgt(Register a, Register b) {
    static constexpr size_t register_width = bit_width<Register>();
'''

    return res + instantiateIfConstexprPattern_intWidth(
        'register_width == {0} && is_equivalent<T, std::int{2}_t>()',
        'return _mm{1}_cmpgt_epi{2}(a, b);'
    )


# Addition/Subtraction ==================================

def add():
    res = '''
  template <typename T, typename Register>
  inline auto add(Register a, Register b) {
    static constexpr size_t register_width = bit_width<Register>();
    static constexpr size_t t_width = sizeof(T) * 8;
'''

    return res + instantiateIfConstexprPattern_intWidth(
        'register_width == {0} && t_width == {2}',
        'return _mm{1}_add_epi{2}(a, b);'
    )

def sub():
    res = '''
  template <typename T, typename Register>
  inline auto sub(Register a, Register b) {
    static constexpr size_t register_width = bit_width<Register>();
    static constexpr size_t t_width = sizeof(T) * 8;
'''

    return res + instantiateIfConstexprPattern_intWidth(
        'register_width == {0} && t_width == {2}',
        'return _mm{1}_sub_epi{2}(a, b);'
    )


# byte mask =============================================

def movemask():
  return '''
  template <typename T, typename Register>
  inline auto movemask(Register a) {
    static constexpr size_t register_width = bit_width<Register>();
    static constexpr size_t t_width = sizeof(T) * 8;

    if constexpr (register_width == 128 && t_width == 8)
      return _mm_movemask_epi8(a);
    else if constexpr (register_width == 256 && t_width == 8)
      return _mm256_movemask_epi8(a);
    else return error_t{ };
  }
'''

def blendv():
  return '''
  template <typename T, typename Register>
  inline auto blendv(Register a, Register b, Register mask) {
    static constexpr size_t register_width = bit_width<Register>();
    static constexpr size_t t_width = sizeof(T) * 8;

    if constexpr (register_width == 128 && t_width == 8)
      return _mm_blendv_epi8(a, b, mask);
    else if constexpr (register_width == 256 && t_width == 8)
      return _mm256_blendv_epi8(a, b, mask);
    else return error_t{ };
  }
'''

def generateMainCode():
    res = ''
    res += section('register_i')
    res += register_i()

    res += section('sizes')
    res += sizes()

    res += section('load/store')
    res += load()
    res += load_ignore_asan()
    res += store()

    res += section('set one value everywhere')
    res += set0()
    res += set1()

    res += section('min/max')
    res += min()
    res += max()

    res += section('comparisons')
    res += cmpeq()
    res += cmpgt()

    res += section('add/sub')
    res += add()
    res += sub()

    res += section('movemask')
    res += movemask()
    res += blendv()

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
