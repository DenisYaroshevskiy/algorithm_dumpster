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

### factoriadic_representation

`compute_factoriadic_representation_length`<br/>
`from_factoriadic_representation`<br/>
`to_factoriadic_representation`

[Wikipedia](https://en.wikipedia.org/wiki/Factorial_number_system)

Converts a number to it's factoriadic representation.<br/>
Go left to to right with increasing digits.<br/>
Zero is always present (0!) - this allows to do the algorithm better.<br/>

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
_guarantied variations don't check for boundaries.

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

### nth_permutation

`nth_permutation`

[Medium blog](https://medium.com/@aiswaryamathur/find-the-n-th-permutation-of-an-ordered-string-using-factorial-number-system-9c81e34ab0c8)

[Wikipedia](https://en.wikipedia.org/wiki/Factorial_number_system)

Gives nth lexicographical permutation of the sequence, assuming that the original sequence <br/>
is sorted. (doesn't rely on the original order in any way).<br/>

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

`copy_revere_iterators_common`
`copy_reverse_iterators_int_vec`

Speed of copying with reverse iterators. On just this loop, superiority of memmove is not showing. <br/>
For example with gcc: http://quick-bench.com/vpLxYCWfverSzlWxFjwuXxELD8Q <br/>

Last time I tried this was very visible for merge/flat_set benchmark - <br/>
if still a thing - will show there.

### merge

`merge_common`<br/>
`merge_int_vec`

Benchmarking merge like algorithms.

### sort

`sort_common`<br/>
`sort_int_vec`

Benchmarking sort like algorithms.

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
