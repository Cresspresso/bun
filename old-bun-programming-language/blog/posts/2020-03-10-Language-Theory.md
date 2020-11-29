
[Previous](./2020-03-03-The-Beginning-of-Bun.md) • [All Posts](../posts.md) • [Next](./2020-04-01-Variable-Binding.md)

10 March 2020

Elijah Shadbolt

# Langauge Theory

The first chapter of [Design and Implementation of Compiler][1] describes language. It describes mathematics and notation that we can use to define languages. This is theory we already know.

- Alphabet - set of symbols, characters, letters, numbers, Unicode code points
- String - finite sequence of symbols over an alphabet
- Grammar
- Language
- Regular Expressions - for example, S → 0|1
	- a* - zero or more a's (kleene closure)
	- a+ - one or more a's (positive closure)
	- a|b - either a or b (union)
- Automata
	- Deterministic Finite Automata (DFA) - state machine which can only transition with a non-empty input string
	- Non-Deterministic Finite Automata (NFA) - nodes can have multiple exit transitions with the same input string
- Derivation Tree (Syntax Tree) - hierarchy of symbols and operators
- Context Free Languages
- Context Sensitive Languages

[1]: Singh, Ravendra, et al. Design and Implementation of Compiler. 2009.
