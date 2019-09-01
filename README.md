This is where I'm going to store all of the little bits of code
that I do for personal projects.

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

Let's say you need to sort smth that's expensive to move.<br/>
You can sort iterators instead and then just rearrange the original sequence based on where everything landed.

This is what this algorithm does - takes the rearranged iterators and makes the values have the ordering as the iterator ordering.

The copy/move versions are trivial.

**The inplace version** needs to know the original number of the underlying iterator (distance(f, it)): <br/>
it can either deduce it by subtracting the original f or it can accept a functor to do this.

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

Optimization with `half_positive` was upstreamed to libc++: [patch](https://reviews.llvm.org/D53994)

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

### half_positive

`half_positive`

Dividing by two is faster for unsigned numbers. <br/>
If I know that the number is positive I can cast to the unsigned.
I have measured - code generated for right shift gave me slower results + I'm not too sure that's legal.

[libc++ commit](https://reviews.llvm.org/D53994)

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

### lift_iterators

`lift_as_vector` <br/>

From a range make a range of iterators. At least for now I always add a couter to<br/>
non-random access iterators in a shape of a wrapper.

This gives me a constnat time distance and position/comparisons.<br/>
This sort of 'lifted' iterators as a result are more powerful than their counterpars without it<br/>
I refer to this concept with an `N` suffix, like `ForwardNIterator` `BidirectionalNIterator`.

This is a minimal requirement for `apply_rearrangment` in place for example.

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

`ValueType`<br/>
`DifferenceType`<br/>
`Pointer`<br/>
`Reference`

Collection of basic template magic stuff.

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

### _function objects

Algorithms wrapped in objects.

### input_generators

`int_to_t`<br/>
`sorted_vector`<br/>
`two_sorted_vectors`

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
`merge_vec`

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
`make_container_of_stable_unique_iota` <br/>
`stable_unique`

`stable_unique` - is a move only type, consisting of zeroed_int for the value + an int<br/>
tag to indicate the origin.<br/>
`make_container_of_stable_unique` - converts vector of integers + tag to a given container.

### zeroed_int

A struct with one int, that zeroes out on a move.

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
