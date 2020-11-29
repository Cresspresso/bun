
[Previous](./2020-04-08-Software-Design-Pillars.md) • [All Posts](../posts.md) • [Next](./2020-05-06-Writing-a-Paper.md)

14 April 2020

Elijah Shadbolt

# Given C++ Features
Given that C++ is the destination language we will compile Bun code to, we should consider the available features.

## Preprocessor
- `#include "filename"`
- preprocessor definitions: `#define placeholder_name replacement_text`
- macros: `#define macro_name(arguments) replacement text with arguments`
- `#if`, `#else`, `#endif`
	- `defined(name)`
- headers `.h` and implementation `.cpp` files
- header guards

## Program Scopes
- macro...
- namespace (global)
	- namespace...
	- using namespace...
	- using typename...
	- using overload...
	- type...
	- variable...
	- compile time constant variable...
	- function...
		- (Can be forward declaration or full definition)
		- (Either: concrete, template, or template specialization)
		- template parameter...
		- variable parameter...
		- block

### Type
Can be forward declaration or full definition.

Either:
- struct/class
	- (Either: concrete, template, or template specialization)
	- template parameter...
	- using namespace...
	- using typename...
	- using inherited overload...
	- field...
	- method...
	- virtual method... either:
		* abstract
		* virtual
		* override
		* final
	- ctor/dtor...
	- static variable...
	- static function...
	- inherited class...
	- type...
- union
	- (Either: concrete, template, or template specialization)
	- template parameter...
	- ctor/dtor...
- enum
	- inherited integer type

### Block
- using namespace...
- using typename...
- using overload...
- type... (not template)
- variable...
	- initialization expression
- action...
	- void expression that mutates state
- flow control block... either:
	* block: {}, do{}while(0);
	* branch: if, else
	* loop: while, for
	* try-catch
- flow control statement... either:
	* return
	* break
	* continue

### Expression
Either:
- nested expression in parentheses()
- literal value
- variable getter
- binary operator
- unary operator
- function call
- lambda function closure
	- capture clause
		- captured variable...
	- parameter...
	- block

## Querks of C++
- Reference semantics
- Raw array semantics
- Class lifetime semantics
	- L-value and r-value references
	- Perfect Forwarding of Arguments
	- Return Value Copy Elision
	- Rule of 5
- Const Correctness for Methods
- Noexcept Specifications for Functions
- Member access permissions
- Function automatic implementation `= default` or `= delete`
