This is where I'm going to store all of the little bits of code
that I do for personal projects.

## Algorithms

Collection of mostly stl-like algorithms and some general purpose data-structures.

### memoized_function

A wrapper around a callable, that stores outputs for previously computed inputs.
Not terribly efficient - currently uses std::map to implement storage.

### to_factoriadic_representation

[Wikipedia](https://en.wikipedia.org/wiki/Factorial_number_system)

Converts a number to it's factoriadic representation.<br/>
Go left to to right with increasing digits.<br/>
Zero is always present (0!) - this allows to do the algorithm better.<br/>

0 => { 0 }<br/>
1 => { 0 1 }<br/>
2 => { 0 0 1 }<br/>
3 => { 0 1 1 }<br/>

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
