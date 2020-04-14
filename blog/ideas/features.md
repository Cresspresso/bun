
14 April 2020

Elijah Shadbolt

# Language Features
We must decide what features we want to provide in Bun.

- what features are builtin/primitive?
- what can be written as part of a standard library?

## Variables
- static variables
- variables local to a function or block
- function parameters
- class member variables (fields)

### Immutable Variables
- compile-time constants
- const correctness
- immutable variables (runtime const)

## Functions
- static functions
- class member functions (methods)
- closures (lambdas)
	- explicit capture clause
	- implicit capture (gc environment)

## Structs and Classes
- member access permissions (private, public)
- initialisation (constructor)



## Data Types

### Multiplicative Data Types
Stores data of type A `and` data of type B.
- Struct (descriptive field names)
- Tuple (compile time subscript index)
	- Static Array (tuple with all fields same type, runtime subscript index)
- BitSet / Enum Flags (it can be 0b0001 and 0b0010 at the same time)

### Collections
aka Dynamic Multiplicative Data Types
- Dynamic Array (variable length)
- Array List (Dynamic Array with Capacity)
- Linked List
- Hash Set
- Dictionary
- Stack
- Deque

### Additive Data Types
Stored data is of type A `or` type B.
- Value based additive types (it is either 0 or 1, not 0 and 1)
	- Integer
	- Enum
	- Boolean
	- Floating Point Number
	- Pointer (Memory Address)
- Union (descriptive field names)
- Variant (tagged Union, i.e. Union with Enum to indicate which field is active)
	- Either (Variant with two fields)
		- Option (Variant for a value or a nil sentinel)



## Error Handling

### Exceptions
- exception with runtime polymorphism
- exception with compile time polymorphism
- try, catch
- try, catch, finally

### Return Values
- return code
	- int negative for error, positive for expected result value
	- bool or enum error codes, 0 for success, not 0 for error
	- return code and reference out parameters
- null sentinel
- multiplicative data type: return both a value and an error code
- additive data type: return either a result value or an error value



## Memory Management

### Raw Pointers
- manual alloc/free
- destructor
	- RAII unique_ptr
	- shared_ptr / ref count
- lifetime semantics (copy ctor, move ctor, copy assign, move assign, swap)
- volatile read/write

### Garbage Collection
- reference types, garbage collected (class)
- value types, for variables and fields (struct, enum, int); no move, only copy
- unsafe blocks for manual raw pointer management



## Polymorphism

### Runtime Polymorphism
- class with virtual methods (abstract or virtual class)
	- interfaces
- virtual multiple inheritance (diamond problem solved)
- boxed value types (for language with garbage collection)
- pointer to base class, yet runtime type is a derived class

### Compile Time Polymorphism
- function overloads
	- operator overloads
- template functions
- template classes
	- explicit template specialization
- auto keyword and decltype(auto) keyword
- concepts
