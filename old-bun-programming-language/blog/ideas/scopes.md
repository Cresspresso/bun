
14 April 2020

Elijah Shadbolt

# Scopes
Ideas

## Project Source Code
- folder
	- folder...
	- source file...

## Solution
- macro, preprocessor definition...
- namespace
	- namespace...
	- static variable... either:
		* mutable
		* compile time constant
		* runtime immutable
	- data type... either:
		* additive
		* multiplicative
		* alias
	- static function...
		- parameter...
		- function block
			- local variable...
			- local function...
			- flow control block... either:
				- branch (if, if/else, if/else if)
				- loop (while, for)
				- nested block (run)

### Variable Scopes
- static (global)
- stack (local)
- field (instance)

### Function Scopes
- static (global)
- stack (closure)
- method (instance)

### Variable Mutability
- mutable
- runtime immutable
- compile time constant (requires static)
