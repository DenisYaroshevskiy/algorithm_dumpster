This is where I'm going to store all of the little bits of code
that I do for personal projects.

There are the following libraries-ish.
* algo - stl like algorithms, data-structrures
* bench_generic/bench_runnable - benchmarking utils/just benchmarks
* compiler - I only really use clang, but I chose to hide some extensions
             behind macros.
* simd - very minimalistic simd wrapper library. Only works for AVX2.
* unsq - implementing some stl like algorithms with std::execution::unseq
         could've been done (for AVX2).

Every diretory has a secion in this README.

## License and usage

Everything is under Apache-2 - as far as I understand it:
"you can use it as long as you don't claim it's yours".

I use a ton of things from other people - I tried to live links in the README:<br/>
"this is where I took it", which I hope is good enough.

Feel free to contact me if you have an issue with how I handled your work <br/>
(or if you have other reasons).

## Benchmarking results

Visualized benchmarking results can be found at:<br/>
https://denisyaroshevskiy.github.io/algorithm_dumpster/

## Algo

Collection of mostly stl-like algorithms and some general purpose data-structures.

### advance_up_to

`advance_up_to`

Same as std::advance but also accepts 'l' to check for it.
Returns both where it got and how much of 'n' was left.

I don't know how this is suppose to work for negative advance.
So it's undefined.

### apply_rearrangment

`apply_rearrangment`<br/>
`apply_rearrangment_copy`<br/>
`apply_rearrangment_move`<br/>
`apply_rearrangment_no_marker`

_See **position** on concepts)_

Taking a range of posittions, rearrange the elements pointed to by those positions in the same order as the positions.

Copy/Move versions put the rearranged elements into the new memory.
(trivial to write)

**The inplace version**

There are two inplace versions: `apply_rearrangment` and `apply_rearrangment_no_marker`.

The thing is the algorithm needs to know which iterators were already processed.
If there is a marker it can use to signify iterators that were already moved (like last or nullptr), it can be faster.

The iterator range get's destroyed (see - the algorith needs to mark what was processed).

Implemenantation is based on the ideas from Elements of Programming, section 10

Lemma 10.6 Every element in a permutation belongs to a unique cycle.<br/>
Lemma 10.7 Any permutation of a set with n elements contains k ≤ n cycles.<br/>
Lemma 10.9 Every permutation can be represented as a product of the cyclic permutations corresponding to its cycles.<br/>

Basicly - it just applies the cycles through all of the permutations.

We can also **move away and then move back** (`apply_rearrangment_move` to a buffer and then `move`).<br/>
I did measure that - for ints/doubles it was faster. However - for strings - the inplace version with marker did better.

### binary_counter

`add_to_counter`<br/>
`reduce_counter`<br/>
`binary_counter_fixed`

[Efficient programming with components](https://youtu.be/yUZ3y5w3f0o)

_TODO_: `binary_counter` from the course, vector based.

Idea from the Efficient programming with components course, generalized binary counter.

Binary counter is smth like: 00000 => 00001 ==> 00010 ==> 00011 ==> 00100
Now, if you replace the add in the power of two with an arbitrary reduction operaton,<br/>
you get a very interesting device.

I wanted to use this (as was one of the ideas from the course) in stable_sort to convert
a 'divide and conqure' merge sort to a bottom up merge sort.

I chose for a different implementation for `reserve` - instead of increasing the vector capacity, it adds zeroes to the front.
It gives me a natural and consistent impletemtation between vector and array.

I also chose to have a type of `zero` to be convertible to `ValueType` instead of directly `ValueType` because this gives me support to move only types. (Like reducing `unique_ptr`s with `nullptr` as zero).

_TODO_: Since we know limits on logarithms, we can implement 'add no overflow' if that's
useful. Maybe not since 'counting' is nothing compared to the cost of the reduction operation. Still.

### binary_search

`partition_point_n`<br/>
`partition_point`<br/>
`lower_bound_n`<br/>
`lower_bound`

_TODO_: `upper_bound`/`equal_range`/`_counting`

Implementation of standard binary search algorithms.<br/>
The n versions based on ideas from [Efficient Programming With Components](https://youtu.be/MHHLKuvfBwQ)<br/>

The n versions are a bit weird - they do not return the distance from the beginning
or to the end, which means that they have questionable usability unless one knows<br/>
both `n` and `last`.

_TODO_: couting versions that return the distance to the end by doing
extra work. This can be more efficient then just computing the distance.

Optimization with `half_nonnegative` was upstreamed to libc++: [patch](https://reviews.llvm.org/D53994)

### binary_search_biased

`partition_point_biased`<br/>
`lower_bound_biased`<br/>
`partition_point_biased_expensive_pred`<br/>
`lower_bound_biased_expensive_cmp` <br/>
`partition_point_hinted` <br/>
`lower_bound_hinted` <br/>
`partition_point_linear` <br/>
`lower_bound_linear` <br/>
<br/>
`point_closer_to_partition_point`<br/>
`point_closer_to_lower_bound`<br/>
`point_closer_to_upper_bound`

_TODO_: `upper_bound`/`equal_range`/`_n`

My [blog post](https://medium.com/@denis.yaroshevskij/between-linear-and-binary-search-8d21877cfce5)
on the subject (the measurements are outdated):<br/>

Or a [meetup video](https://skillsmatter.com/skillscasts/12831-between-linear-and-binary-search).

My variations on the galloping (exponential) search. <br/>
Non `_expensive` trade off to do more predicate invocations in order<br/>
to remove boundary checks.<br/>
`_hinted` variations instead of being biased to the first element, are
biased to a `hint`. Requires `BidirectionalIterator`. <br/>
`_linear` variations use find_if to find the lower bound. By my measurements should not be useful, at least for random access.
<br/>
`point_closer_to` - returns element somewhere to the left of the partition point.
Proved to be useful for merge algorithm.
The input range has to be non-empty. <br/>
Never returns the end. <br/>
ForwardIterator support is questionable. <br/>
In reality now just doesn't do the bigger jumps, if the middle if to the left of
the partition point, don't loop - just return. Because this is a very rare case - ignoring it and just going back to the main merge was faster.

### comparisons

`less_by_first`

Collection of general purpose comparator functors.

### container cast

`container_cast`

A function that converts data from one container to another.
Like: `container_cast<std::list>(v)`. Will move values from an rvalue container.

### copy

`copy`<br/>
`copy_backward` <br/>
`copy_n` <br/>
`copy_backward_n` <br/>

Same as std::copy/std::copy_backward but covers more optimizations.<br/>
Tried to upstream it but failed: [patch](https://reviews.llvm.org/D38653).

[Small presentation](
https://docs.google.com/presentation/d/1JcpsUq2s-cK_3A4sco23YRrsSKMV4EoeD8s4u8f_q0w/edit?usp=sharing)

For now non-constexpr, requires extra work.<br/>
Implement necessary bits for libc++ [pathc](https://reviews.llvm.org/D63063).(not accepted as of yet)

There was also a bug, that std::copy optimization didn't work at all<br/>
https://bugs.llvm.org/show_bug.cgi?id=40575<br/>
In the sandard library version I use it's not yet in.

_NOTE_: I have seen cases where this is a good win, however - my microbenchmark of just copy_reverse_iterators doesn't show it.<br/>
Will update when I migrate propper benchmark in this repo.

`_n` versions are for range and size.<br/>
_NOTE_: unlike std I return the last input iterator.
_NOTE_: copy_backward_n accepts LAST as it's only input iterator.

### half_nonnegative

`half_nonnegative`

(Name is taken from EOP)
Dividing by two is faster for unsigned numbers. <br/>
If I know that the number is positive I can cast to the unsigned.
I have measured - code generated for right shift gave me slower results + I'm not too sure that's legal.

[libc++ commit](https://reviews.llvm.org/D53994) (named half_positive).

### factoriadic_representation

`compute_factoriadic_representation_length`<br/>
`from_factoriadic_representation`<br/>
`to_factoriadic_representation`

[Wikipedia](https://en.wikipedia.org/wiki/Factorial_number_system)

Converts a number to it's factoriadic representation.<br/>
Go left to to right with increasing digits.<br/>
Zero is always present (0!) - this allows to do related algorithms better.<br/>

```
0 => { 0 }
1 => { 0 1 }
2 => { 0 0 1 }
3 => { 0 1 1 }
```

Representation comes out left is smallest digit.

### factorial

`factorial`

Computes a factorial of an input number.

### find_nth

`find_nth_guarantied`<br/>
`find_nth_if_guarantied`

Similar to std::find/find_if but returns nth_entry instead of the first one.
`_guarantied` variations don't check for boundaries.

Indexing is from 0 - find 0th returns the first encouted element.

### positions

`lift_as_vector` <br/>

Position concept:

Idea:
This concept arises from, so far 3 algorithms from this library:
`apply_rearrangement`, `stable_sort_lifting`, `nth_permutation`.

For these algorithms have in common that you take a range, make pointers
to all of the elements and then do smth with them.

For actually`apply_rearrangement` (and therefor `stable_sort_lifting`) we
need to know how the original elements were ordered.
If we do an operation on the contigious range (like a vector), we can totally use pointer math for this.
We can extend that to arbitrary RandomAccessIterators - it's sufficient to
know the base where they begin.
However, we can also apply the same algorithms for either randomly distributed elements (like: select a bunch of things out of a collection), or - many algorithms require you to mark or remove elements (like nth_permutation).

Syntactically positions satisfy everything about RandomAccessIterator,<br/>
except for ++,--, +=, [] => you cannot go from one position to another.

Canonical models are: Pointers, ForwardIterator + Number, Pointer + Number.

There are two related definitions I use: <br/>
`base` - the position corresponding to the first element,<br/>
`marker` - the special tag value - position that doesn't mean anything, that is useful for some algorithms.

So far `marker` is just position that we got from `last`, because I have been dealing with lifting positions from ranges. However - it can be relaxed in general case to be `nullptr` with arbitrary `N`for example.

`lift_as_vector` - takes a range and returns a vector of `positions` + `base` and `marker` value.

### registry

`registry` <br/>
`stable_registry`

(_TODO_ just wrote the readme, so far didn't implement)

Registry - storage for objects with unqiue keys.<br/>
Difference between stable and non-stable is:
a) stable does not reuse keys <br/>
b) iteration retuns elements in the same order they were put in.

See Sean Parent's [planery](https://youtu.be/ejF6qqohp3M) on "Russian Coatchecking Algorithm" (couldn't google it though) on how stable regisrty is implemented.

Also - Sean Parent uses optionals for elements. I don't do that - I have a 'dummy' argument. There are a couple of ways one could go about 'no element' - one - use equality comparison on the element with the dummy, the other - store information in the key. I chose the key.

Another thing is - we can use either one array of pairs or two parallel arrays.
I chose one array because it's much easier. Also - I did measure this at some point -
the effect of one integer on binary search is minimal.

(Not implemented yet): slot_map - is the idea behind a non-stable (or just registry.
_TODO_: maybe I want to support iterations to reduce key reuse in a registry.
On the other hand - it's not really a registry - maybe it should just be a slot-map.
[Allan Deutsch on Slot map](https://youtu.be/-8UZhDjgeZU)

### strcmp

`strmismatch` <br/>
`strcmp`

Implementation of std::strcmp using simd. <br/>
For `v1::` Tried to be as close as possible to:
https://opensource.apple.com/source/Libc/Libc-825.40.1/x86_64/string/strcmp.s.auto.html

The main one doesn't fall back to checking one by one.

Allows to pick how many bytes to process 16 or 32 at a time.

### strlen

Implementation of an std::strlen from a C standard library using simd. <br/>
Complelty based on: https://stackoverflow.com/questions/25566302/ but allows <br/>
to choose between how many bytes are processed at once (currently 16 and 32).

### shuffle_biased

`shuffle_biased`

Algorithm of questionable quality at the moment. <br/>
The idea is that we want the probability of an element being distributed within certain limit to be bigger.

### memoized_function

`memoized_function`

A wrapper around a callable, that stores outputs for previously computed inputs.
Not terribly efficient - currently uses std::map to implement storage.

### merge

`merge`<br/>
`merge_expensive_cmp`

Variations on std::merge - tried to contribute the `merge` one:<br/>
[patch](https://reviews.llvm.org/D63063)

`merge_expensive_cmp` only useful if you inline the comparison and it's big.<br/>
The reason for it's existance is that `merge` unrolls + 1 extra invocation of the comparator. <br/>
For some reason my latest benchmarks don't show how it's superior to `std::merge`, <br/>
some of my previous benchmarks did, see presentation mentioned in `merge_biased`.

### merge_biased

`merge_biased_first` <br/>
`merge_biased_second`

_TODO_: migrate set unions.

Variation on std::merge that falls back on binari-ish search when it suspects
that there is a big a gap of elements from the range it's biased towards.

[Presentation from the meetup](
https://docs.google.com/presentation/d/1675lZkaJ2FcH9wwdUPYptFGnV_A_TW4tAyObIHGBYgs/edit?usp=sharing)

### mersenne_primes

`mersen_primes_int32`

Constants representing Mersenne primes. <br/>

### move
`move`<br/>
`move_backward`<br/>
`move_n`<br/>
`move_backward_n`

See copy

### nth_permutation

`nth_permutation`

[Aiswarya Prakasan's blog](https://medium.com/@aiswaryamathur/find-the-n-th-permutation-of-an-ordered-string-using-factorial-number-system-9c81e34ab0c8)

[Wikipedia](https://en.wikipedia.org/wiki/Factorial_number_system)

Gives nth lexicographical permutation of the sequence, assuming that <br/>
the original sequence is sorted.<br/>
The algorithm itself doesn't rely on comparisons, an can be used<br/>
o go through all of the permutations with some step starting from any permutation.<br/>
(Do not know a good usecase for this yet).

Permuntations are counted from 0.

_NOTE_: One has to always start from the first sequence, this **DOES NOT MAKE SENCE**:
```
nth_permutation(f, l, o, n);
nth_permutation(f, l, o, n);
```

_NOTE_: Since permutation number might be really big for even small range can be huge,<br/>
consider using big number library.

_NOTE_: algorithm can potentially be done in place but it's hard and unneccary.

Allocates O(distance(f, l)) memory.

### stable_sort

`stable_sort_n_buffered`<br/>
`stable_sort_n_sufficient_allocation` <br/>
`stable_sort_sufficient_allocation`<br/>
`stable_sort_lifting`

Also
`stable_sort_n_buffered_std_merge` <br/>
`stable_sort_n_sufficient_allocation_std_merge`<br/>
`stable_sort_sufficient_allocation_std_merge`

In many respects, this is my crack at the last task in the <br/>
[Efficient programming with components](https://www.youtube.com/playlist?list=PLHxtyCq_WDLXryyw91lahwdtpZsmo4BGD)

(work in progress).<br/>

`stable_sort_n_buffered` - idea originally from [here](https://github.com/rjernst/stepanov-components-course/blob/375bcb790ee40020ff639e0b8ddec0cfe58ba27a/code/lecture17/merge.h#L59).

`_lifting` - lifts a vector of iterators, sorts that and then applies the rearrengment.

`_std_merge` versions - more to check how important it is to use my merge over std one.

### type functions

`ArgumentType` <br/>
`bit_size` <br/>
`DifferenceType`<br/>
`ForwardIterator`/`BidirectionalIterator`/`RandomAccessIterator` <br/>
`IteratorCategory` <br/>
`MoveIterator`/`ReverseIterator`<br/>
`Pointer`<br/>
`Reference` <br/>
`uint_t` <br/>
`ValueType`

Collection of basic template magic stuff.

### unroll

`unroll`

Repeat the same operation multiple types without the loop.

### uint_tuple

`uint_tuple`

_implemented by Oleg Fatkhiev(@ofats)_

_TODO: benchmarks for comparison functions_ <br>
_TODO: warn or fail on using bigger type_ <br>
_TODO: report to clang slow codegen for pair bug_ <br>

A tuple that stores unsigned numbers into a single unsgined integer of an appropritate size.<br/>
The main benefit is the abitlity to compare them with one machine instruction.
Also possibility to pack data sometimes can be very useful.

Thanks to Philip Trettner for help with paddings and comparisons.
Some inspiration from [Andrew Alexandrescu's talk, Writing Quick Code in C++, Quickly](https://youtu.be/ea5DiCg8HOY) - about Tudor Bosman's bitfield stuff.


Tried seters/getters in a pair (same bit size for both) using uint_tuple and std::pair<br/>
Mostly identical code, except when setting two integers simoteniously.<br/>
In this case, uint_tuple is significantly faster - see zip_to_pair benchmark.<br/>
Reported this to clang: https://bugs.llvm.org/show_bug.cgi?id=43864 <br/>
Godbolt to play around: https://gcc.godbolt.org/z/YfdnZo


## Bench (generic/runnable)

Collection of benchmarks and some tools to do benchmarks.
Generic - templates that do things and reusable things.
Runnable - Specificly crafted *.cc files, each one of which is conerted to it's own benchmark executable (sometimes multiple).

Depends on google benchmark and boost.

_NOTE_: I don't know how to CMAKE - dependencies are found in installed folders.

### counters_benchmark

`counting_wrapper`<br/>
`clear_counters`<br/>
`counters_to_json_dict`<br/>
`counters_writer`<br/>
`counting_benchmark`

Utils to count operations in the benchmark.

### declaration

`BENCH_DECL_ATTRIBUTES`

Still in the hopes to find some magic attibutes that would help with the code alignment.
For now - just noinline, to help when looking at assembly.

### _function objects

Algorithms wrapped in objects.

### input_generators

`int_to_t`<br/>
`sorted_vector`<br/>
`two_sorted_vectors`<br/>
`nth_vector_permutation`

Utils to generate data for benchmarks.

### apply_rearrangment

`apply_rearrangment_common`<br/>
`apply_rearrangment_vec`

Benchmarking `apply_rearrangment` algorithms.

### copy

`copy_revere_iterators_common`<br/>
`copy_reverse_iterators_int_vec`

Speed of copying with reverse iterators. On just this loop, superiority of memmove is not showing. <br/>
For example with gcc: http://quick-bench.com/vpLxYCWfverSzlWxFjwuXxELD8Q <br/>

Last time I tried this was very visible for merge/flat_set benchmark - <br/>
if still a thing - will show there.

### lower_bound

`lower_bound_common`<br/>
`lower_bound_vec` <br/>
`lower_bound_vec_first_5_percent`

Benchmarking lower_bound like algotihmms.<br>
`_first_5_percent` - benchmark for 'biased case' - results are close to the beginning.

### merge

`merge_common`<br/>
`merge_vec` <br/>
`merge_with_small`

Benchmarking merge like algorithms.
Merge with small - benchmarks merge of a big first range with a small second one.

### sort

`sort_common`<br/>
`sort_int_vec`

Benchmarking sort like algorithms.

### zip_to_pair

`use_pair`<br/>
`use_uint_tuple_first_second`<br/>
`use_uint_tuple_second_first`<br/>
`zip_to_pair_common`<br/>
`zip_to_pair_bit_size`

Benchmarking popluating a number of elemts into a vector of pairs using different types of pairs.<br/>
Pairs are of the same uint type for both elements.<br/>
This was to measure wether a different codegen for uint_tuple vs std::pair is better or worse.<br/>
On my machine - noticebaly better.

This benchmark on Quick-bench: http://quick-bench.com/aDq3iN3dpi9VWQc8XSd6o7Hlzl4<br/>

## simd

A very cut down simd wrapper library that I feel in as need. <br/>
Existing wrapper libraries didn't work for me, looked at:<br/>

* nsimd
* xsimd
* tsimd
* VC

Only work with integer types, everything Intel specific, needs at least AVX2.
Will see how this works out for me, I'd like to do simd optimized algorithms.

### bits

`count_trailing_zeros`<br/>
`lsb` <br/>
`lsb_less` <br/>
`set_lower_n_bits` <br/>
`set_highest_4_bits` <br/>

Some bit maniputation for regular integers.

`lsb_less` - pretend that the more significant bits are less significant<br/>
and do comparison that way.

`set_highest_4_bits` - helper to do comparisons for unsigned numbers<br/>
using signed ariphmetic.

### mm

`register_i<bits>` <br/>

`alignment` <br/>
`bit_width` <br/>
`byte_width` <br/>

...
copied intrinsics

A very very lowlevel abstruction on top of intel intrinsics: https://software.intel.com/sites/landingpage/IntrinsicsGuide <br/>
The only thing it allows is for the user to template over bit width.
Tried hard to preserve intel naming.

The code is generated from `mm_generate_opeations.py`.

So - instead of `_mm_load_si128` or `_mm256_store_si256` we can `mm::load(const register_i<128>)` <br/>

Also some type traits to just tell how many bits the register have etc.

### mm_operations_generator

python script to generate mm.h

### pack

`pack<T, W>`<br/>
`register_t<pack>`<br/>
`vbool_t<pack>`<br/>

`equal_pairwise(pack, pack)`<br/>
`greater_pairwise(pack, pack)` <br/>

`equal_full(pack, pack)`<br/>
`less_lexicographical(pack, pack)`<br/>
`operator==/!=/</>/<=/>=`

`add_pairwise` <br/>
`sub_pairwise` <br/>
`operator+/-/+=/-=`

`load<pack>(const T*)`<br/>
`load_unaligned<pack>(const T*)`<br/>
`store(T*, pack)`

`set_all<pack>(scalar)`<br/>
`set_zero<pack>`

`blend(pack, pack, vbool)`

`cast<pack>` </br>
`cast_elements<T>` </br>
`cast_to_bytes` </br>
`cast_to_signed` </br>
`cast_to_unsigned` </br>

`and_` <br/>
`or_` <br/>
`xor_` <br/>
`not_x_and_y` <br/>
`not_` <br/>
`operator&|^~`

`top_bits` <br/>
`get_top_bits` <br/>
`ignore_first_n` <br/>
`ignore_last_n` <br/>
`first_true` <br/>
`all_true` <br/>
`all_true`<br/>

`end_of_page(addr)` <br/>
`previous_aligned_address<Pack>(addr)`<br/>

`compress_mask_for_shuffle_epi8` <br/>
`compress_mask_for_permutevar8x32` <br/>
`compress_store_unsafe(T*, pack, mmask) -> T*` <br/>
`compress_store_maskedT*, pack, mmask) -> T*` <br/>

`swap_adjacent_groups<group_size>(pack) -> pack` <br/>

`spread_top_bits(top_bits) -> pack` <br/>

`reduce(pack, op) -> pack` <br/>
`replace_ignored(pack x, ignore (?), pack with) -> pack` <br/>

`to_array(pack) -> std::array` <br/>

A simd::pack of integer values, incapsulating `mm::register`.<br/>
The only member is a corresponding register, which is public so that we can implement different operations on top. <br/>

There are some type functions on top like `register_t` to get the `mm` register and `vbool_t` to get the correcsponding simd mask type.

Some simd wrappers support direct `operator[]` to access specific elements. I decided against it for now because I think that I want loads/stores to memory to be explicit.

`operator==/!=/</>/<=/>=`

Many simd wrappers chose to implement these to return `vbool`. I don't think<br/>
that this works works well with the rest of C++ ecosystem - so operators work the same<br/>
as for containers. There are `pairwise` versions of similar operations when you need them.

`load/store`

Default load, store require aligned pointers.

`end_of_page`, `previous_aligned_address`

We are allowed to read the memory we didn't directly allocated if it's within
the same page.<br/>
We can read till `end_of_page` and we always OK to read from `previous_aligned_address`.<br/>
Based on the idea from `strlen`, see more here: https://stackoverflow.com/questions/25566302/vectorized-strlen-getting-away-with-reading-unallocated-memory<br/>
Code: https://opensource.apple.com/source/Libc/Libc-997.90.3/x86_64/string/strlen.s.auto.html<br/>

`blend(pack, pack, vbool)`

Same as intel, if true take second.

`compress_mask`

In avx-512 there are intrinsics `*compressstore*` - which are very useful. <br/>
This is an approximation of this for avx-2. <br/>
There are 2 avaliable shuffle instructions:
`_mm_shuffle_epi8` - works for anything for 128 bit register
and `_mm256_permutevar8x32_epi32` - works for 256 bit register, but the types have to
be at least 4 bytes.

The solutions is based on: https://stackoverflow.com/a/36951611/5021064 <br/>
This was also very instrumental: https://stackoverflow.com/questions/18971401/sparse-array-compression-using-simd-avx2

Output: a pair: <br/>
 1) compressed array of indexes where bits were true, followed by zeros. <br/>
 2) number of selected elements - computed as a by-product and is a very useful thing when using compress.

_TODO_: This is purely an mm hand written code, that doesn't get generated from python. <br/>
I need to figure out what do I do with code like that - probably mm should become it's own <br/>
stand alone folder and this should go there. It's not really a part of the pack interface.

_TODO_: right now I only use BMI2 extensions (2 different implementations). There is also a possibility to use precomputed masks. I need to measure that at some point.

`compress_store_unsafe/compress_store_masked`

Same semantics as `*compressstore*` from AVX-512 emulated in AVX2. <br/>
`compress_store_masked` is one for one the same semantics, <br/>
`compress_store_unsafe` relies on the ability to write the whole register => will override whatever is there for at most the length of the register.

TODO: analyze if I can use `_mm256_permutevar8x32_epi32` for `compress_store_masked`.

`top_bits`

top bits is an abstruction over the result of movemask (get_top_bits in this library).
In many respects alternative to vbool, that is useful in cases where vbool isn't. <br/>
Potential usecase: when looking for element via simd - use `first_true(top_bits)` to find at what position. <br/>
One useful design decision: ignore_*_ functions have overload that doesn't take a parameter of how much to ignore, which allows usage in a templte context.

`swap_adjacent_groups<group_size>`

If group_size == 1 => you get elements next to each other swapped (like 0s and 1st).
For group_size == 2 it's going to be [0, 1] swapped with [2, 3]
etc -> up to group size == width / 2. (only supports powers of 2).
Main driving horse for reduce.

`spread_top_bits(top_bits) -> pack` <br/>

Reverse to get_top_bits. Well - will spread the bit into every bit of the element.
2 fundamental versions: one for bytes and an optimized one if we have under 8 elements (just turned out like that - basically we can do better if the mask can fit into an element).
Relevant stack overflows:
https://stackoverflow.com/a/24242696/5021064
https://stackoverflow.com/a/36491672/5021064

`reduce`

Given a pairwise operation (needs to associate and commute) -> compute a reduction.<br/>
Operations example: `+`, `min_pairwise`.<br/>
Returns a pack with all the elements equal to result.

`replace_ignored(pack x, ignore (?), pack with) -> pack`

Helper on top of blend/spread_top_bits to replace values marked to be ignored with a given one.

`to_array(pack) -> std::array`

Converts pack to std::array.

## Test

Tests for everything. Has a few general purpose test utilities though.

### binary_search_generic_test

Generic tests for binary searches.

_TODO_: constexpr tests.

### merge_generic_test

Generic tests for merge variations on the merge algorithm.

_TODO_: write a test for input iterators.

### stability_test_util

`copy_container_of_stable_unique`<br/>
`make_container_of_stable_unique`<br/>
`make_container_of_stable_unique_iota` <br/>
`stable_unique`

`stable_unique` - is a move only type, consisting of zeroed_int for the value + an int<br/>
tag to indicate the origin.<br/>
`make_container_of_stable_unique` - converts vector of integers + tag to a given container.

### zeroed_int

A struct with one int, that zeroes out on a move.

## Unsq

SIMD implementation of some of the stl like algorithms, using AVX2 extensions.
For now only support integer types and only types that can directly represented
in the simd register. And pointers, pointers should work as well. But generally:
int/8/16/32/64, uint/8/16/32/64.

Uses simd library to actually do pack operations.

### concepts

_TODO_

### find

`find_if_unguarded`, <br/>
`find_unguarded` <br/>
`find_if` <br/>
`find` <br/>

`_unguarded` is a generalization on std::strlen from a C standard. <br/>
Complelty based on: https://stackoverflow.com/questions/25566302/

### iteration

`iteration_aligned_unguarded` <br/>
`iteration_aligned`

Isolation of specific iteration patterns that are used in multiple algorithms.
Accepts a predicate that accepts an equivalent iterator (where to load from), and an optional ignore mask (will be invoked with 2 parameters if no mask). Predicate should return true if the iteration should be aborted.
Whether one can use aligned or unaligned loads depends on the algorithm.

`iteration_aligned_unguarded` - iteration over one range, using aligned reads.
Every address is loaded at most once. Algorithm will figure out a mask for a first load might be partial, after that there are no masks.
Examples of algorithms that use this: `find_if_unguarded`

`iteration_algined`- iteration over one range, using aligned reads. Will stop when the iteration when hits `last`. First and last reads might be partial (it can be the same read).

### reduce

`reduce` <br/>
`min_value -> optional<ValueType<I>>` <br/>
`max_value -> optional<ValueType<I>>`

Implementation of a varations on `std::reduce`.
Unlike `std::reduce` can't accept an arbitrary init value - you need
to supply a `zero` - a value that can be reduced with anything and
won't affect the result.

Default value is `ValueType<I>{}` default opeartion is `simd::add_pairwise`.

`min_value`/`max_value`

Usage of `reduce`. Do not accept an operation - since that won't be less but `min_pairwise`/`max_pairwise` equivalents. The point is that you can do these faster then `min_element` because tracking an index is tricky.

### remove

`remove` <br/>
`remove_if`

Implementation of std::remove/std::remove_if.<br/>

So far speed results are all over the place  <br/>
I don't yet know why, I can see a ton of alignment issues at the very least with std::remove, need to deal with that first.


Using bigger register to do simd operations might be questionable,<br/>
since the main operation is compress (before measurements guess) and<br/>
it actually splits the stored register in 2, though might make sense for<br/>
expensive predicate or if I get around to implementing compress, where possible<br/>
more efficiently for bigger types.

There is a tradeoff between doing unaligned loads and aligned loads but with<br/>
one more compress, I don't know yet where it is.

Another trade off is to first do find (for the first true) and only then<br/>
do stores. In an std::remove this is a requirement since self-move assignment,
however for a simd one it's not, so, at least for now, I don't do the first find.

## Scripts

### benchmark visualization

To use the script insert this line into your html file.

TODO: update

```
<script src="https://cdn.plot.ly/plotly-latest.min.js"></script>
<script type="text/javascript" src="https://cdn.jsdelivr.net/gh/DenisYaroshevskiy/algorithm_dumpster@<COMMIT>/scripts/benchmark_visualization.js"></script>
```

(Replace the commit with the one you one).
It's adviasable to specify the commit otherwise you might run into issues with long term caching.

More information on what's happening can be found here:
https://stackoverflow.com/questions/17341122/link-and-execute-external-javascript-file-hosted-on-github

### run benchmark folder

I generate benchmarks for similar algorithms for the same input into a folder.

This script gets that folder, runs all of the benchmarks in it and drops the output into
a differenet folder.

It also generates the json for benchmark visualisation.

Inputs: folder with benchmarks, folder where to put the result and a path to the template.

### one header

A very hacked together script to generate a single header.
Single headers are stored in the `single_headers` folder.
Mostly to use with godbolt.
