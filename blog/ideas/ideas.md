
14 April 2020

Elijah Shadbolt

# Ideas
- a struct with methods is just a bunch of variables shared between functions as if by forwarding parameters.
- a function closure is just a class with one method.
- all multiplicative data types can access the data fields by subscript index.
	- in the context of a multiplicative data type, a field (subscript index) can be given an alias name.
- in the context of an additive data type, a field (union index) can be given an alias name.
- a function parameter list is just a multiplicative data type (struct).
- C++ would benefit from a way to explicitly specify function parameters by name, for verbosity at the call site, for the case of a constructor storing references without having to do null checks.

## For Loops
```cpp
// C-style for loop
for (init; condition; update) {
	body;
}
// e.g.
for (int i = 0; i < 5; ++i) {
	body;
}

// is the same as
init;
while (condition) {
	body;
	update;
}
```
There is a need for a `defer` block, which executes at the end of the block.
There can bet two kinds of defer blocks, one which executes if no exception is thrown (`defer`), and one which always executes (`defer finally`).

### Use cases of loops
- Repeating an action multiple times.
- Iterating over a range (in a specific direction).
- Iterating over multiple random-access ranges using the same index.
- Multi dimensional ranges (nested loops).
- Node navigation (while there is a current node, find the next node).

- Until terminating condition (while the next element exists).
- Counting (for each integer in the interval `[0, N)`, ascending).
