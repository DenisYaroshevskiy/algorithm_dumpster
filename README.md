This is where I'm going to store all of the little bits of code
that I do for personal projects.

## Algorithms

Collection of mostly stl-like algorithms and some general purpose data-structures.

### find_nth

find_nth_if_guarantied

Similar to std::find/find_if but returns nth_entry instead of the first one.
_guarantied variations don't check for boundaries.

Indexing is from 0 - find 0th returns the first encouted element.

### make_vector_of_iterators

For a range returns a vector with iterators for each element.
Had a choice to use this or pointers, decided for iterators - since they can<br/>
support more relaxed iterator concepts from C++20.

### memoized_function

A wrapper around a callable, that stores outputs for previously computed inputs.
Not terribly efficient - currently uses std::map to implement storage.

### nth_permutation

[Medium blog](https://medium.com/@aiswaryamathur/find-the-n-th-permutation-of-an-ordered-string-using-factorial-number-system-9c81e34ab0c8)

[Wikipedia](https://en.wikipedia.org/wiki/Factorial_number_system)

Gives nth lexicographical permutation of the sequence, assuming that the original sequence <br/>
is sorted. (doesn't rely on the original order in any way).<br/>

Permuntations are counted from 0.

NOTE: One has to always start from the first sequence, this **DOES NOT MAKE SENCE**:
```
nth_permutation(f, l, o, n);
nth_permutation(f, l, o, n);
```

NOTE: Since permutation number might be really big for even small range can be huge,<br/>
consider using big number library.

NOTE: algorithm can potentially be done in place but it's hard and unneccary.

Allocates O(distance(f, l)) memory.

### to_factoriadic_representation

[Wikipedia](https://en.wikipedia.org/wiki/Factorial_number_system)

Converts a number to it's factoriadic representation.<br/>
Go left to to right with increasing digits.<br/>
Zero is always present (0!) - this allows to do the algorithm better.<br/>

```
0 => { 0 }<br/>
1 => { 0 1 }<br/>
2 => { 0 0 1 }<br/>
3 => { 0 1 1 }<br/>
```

`compute_factoriadic_representation_length` - does the algorithm but returns
                                              the resulting lenght instead of values.
`to_factoriadic_representation` - does the conversion

Representation comes out left is smallest digit.

### unroll

Repeat the same operation multiple types without the loop.

## Scripts

### Becnhmarking visualisation scrip.

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
