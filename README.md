This is where I'm going to store all of the little bits of code
that I do for personal projects.

## License and usage

Everything is under Apache-2 - as far as I understand it:
"you can use it as long as you don't claim it's yours".

I use a ton of things from other people - I tried to live links in the README:<br/>
"this is where I took it", which I hope is good enough.

Feel free to contact me if you have an issue with how I handled your work <br/>
(or if you have other reasons).

## Algo

Collection of mostly stl-like algorithms and some general purpose data-structures.

### binary_search

`partition_point_n`<br/>
`partition_point`<br/>
`lower_bound_n`<br/>
`lower_bound`

_TODO_: `upper_bound`/`equal_range`/`_counting`

Implementation of standard binary search algorithms.<br/>
The n versions based on Efficient Programming With Components:<br/>
https://youtu.be/MHHLKuvfBwQ

The n versions are a bit weird - they do not return the distance from the beginning
or to the end, which means that they have questionable usability unless one knows<br/>
both `n` and `last`.

_TODO_: couting versions that return the distance to the end by doing
extra work. This can be more efficient then just computing the distance.

Optimization with `half_positive` was upstreamed to libc++: https://reviews.llvm.org/D53994

### comparisons

`less_by_first`

Collection of general purpose comparator functors.

### container cast

`container_cast`

A function that converts data from one container to another.
Like: `container_cast<std::list>(v)`. Will move values from an rvalue container.

### copy

`copy`<br/>
`copy_backward`

Same as std::copy/std::copy_backward but covers more optimizations.<br/>
Tried to upstream it but failed: https://reviews.llvm.org/D38653

Small presentation:<br/>
https://docs.google.com/presentation/d/1JcpsUq2s-cK_3A4sco23YRrsSKMV4EoeD8s4u8f_q0w/edit?usp=sharing

For now non-constexpr, requires extra work.<br/>
Implement necessary bits for libc++ here: https://reviews.llvm.org/D63063<br/>
(not accepted as of yet)

There was also a bug, that std::copy optimization didn't work at all<br/>
https://bugs.llvm.org/show_bug.cgi?id=40575<br/>
In the sandard library version I use it's not yet in.

_NOTE_: I have seen cases where this is a good win, however - my microbenchmark of just copy_reverse_iterators doesn't show it.<br/>
Will update when I migrate propper benchmark in this repo.

### half_positive

`half_positive`

Dividing by two is faster for unsigned numbers. <br/>
If I know that the number is positive I can cast to the unsigned.
I have measured - code generated for right shift gave me slower results + I'm not too sure that's legal.

libc++ commit: https://reviews.llvm.org/D53994

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

### make_vector_of_iterators

`make_vector_of_iterators`

For a range returns a vector with iterators for each element.
Had a choice to use this or pointers, decided for iterators - since they can<br/>
support more relaxed iterator concepts from C++20.

### memoized_function

`memoized_function`

A wrapper around a callable, that stores outputs for previously computed inputs.
Not terribly efficient - currently uses std::map to implement storage.

### merge

`merge`<br/>
`merge_expensive_cmp`

Variations on std::merge - tried to contribute the `merge` one:<br/>
https://reviews.llvm.org/D63063

`merge_expensive_cmp` only useful if you inline the comparison and it's big.<br/>
The reason for it's existance is that `merge` unrolls + 1 extra invocation of the comparator.

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

### unroll

`unroll`

Repeat the same operation multiple types without the loop.

## Bench (generic/runnable)

Collection of benchmarks and some tools to do benchmarks.
Generic - templates that do things and reusable things.
Runnable - Specificly crafted *.cc files, each one of which is conerted to it's own benchmark executable (sometimes multiple).

Depends on google benchmark and boost.

_NOTE_: I don't know how to CMAKE - dependencies are found in installed folders.

### declaration

`BENCH_DECL_ATTRIBUTES`

Still in the hopes to find some magic attibutes that would help with the code alignment.
For now - just noinline, to help when looking at assembly.

### input_generators

`int_to_t`<br/>
`sorted_vector`<br/>
`two_sorted_vectors`

Utils to generate data for benchmarks.

### copy

`copy_revere_iterators_common`<br/>
`copy_reverse_iterators_int_vec`

Speed of copying with reverse iterators. On just this loop, superiority of memmove is not showing. <br/>
For example with gcc: http://quick-bench.com/vpLxYCWfverSzlWxFjwuXxELD8Q <br/>

Last time I tried this was very visible for merge/flat_set benchmark - <br/>
if still a thing - will show there.

### lower_bound

`lower_bound_common`<br/>
`lower_bound_vec`

Benchmarking lower_bound like algotihmms.

### merge

`merge_common`<br/>
`merge_int_vec`

Benchmarking merge like algorithms.

### sort

`sort_common`<br/>
`sort_int_vec`

Benchmarking sort like algorithms.

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
`stable_unique`

`stable_unique` - is a move only type, consisting of zeroed_int for the value + an int<br/>
tag to indicate the origin.<br/>
`make_container_of_stable_unique` - converts vector of integers + tag to a given container.

### zeroed_int

A struct with one int, that zeroes out on a move.

## Scripts

### benchmark_visualization

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
