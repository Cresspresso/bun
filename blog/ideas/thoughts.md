
8 April 2020

Elijah Shadbolt

# Thoughts

An object is a structure of data upon which specific valid operations can be performed.

An object is an instance of a type.
A type is a template for making objects with the same operations.
In this sense, a type is a class.

Data is state. Data is represented in binary. Data is stored in memory.
Memory consists of free space available to be occupied by objects, and allocated space which hold existing objects.
There are two kinds of memory: stack memory and heap memory.
A location in memory can be referenced by an address.

An object can exist on the stack, stored in a variable.
An object can exist on the heap, indirectly accessable by a pointer stored in a variable.
An object has a lifetime. Some types specify operations for initialisation (construction) and termination (destruction).
A pointer is an object which stores a memory address.

An operation is a sequence of assembly instructions.
An operation can consist of smaller operations.
One such operation is a function call.
In effect, an operation is an algorithm.

A local variable is a name given to a pointer offset relative to the current function on the stack.
A static variable is a name given to a static address in the data section of the assembled machine code.
A function is a name given to a static address in the code section of the assembled machine code.

A variable has a type. It is an instance of a type at this moment during execution.
A variable has a lifetime. Some operations include initialisation (construction) and termination (destruction).
