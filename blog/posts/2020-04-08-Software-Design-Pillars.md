
[Previous](./2020-04-06-Lexer.md) • [All Posts](../posts.md) • [Next](./2020-04-14-Given-Cpp-Features.md)

8 April 2020

Elijah Shadbolt

# Software Design Pillars

This article talks about development practices that software engineers can use while developing software, to have the product communicate to the users (other software developers) to help them understand how its features are used.

This article is inspired by several programming paradigms, including object oriented programming (OOP), functional programming (FP), and data oriented design (DOD). These pillars hold weight regardless of the paradigm they are used in.

In this article, the word "object" refers to self-contained things that represent a well-defined concept, or are used to perform well-defined tasks. Examples include functions (algorithms) and class instances (with operations).

Everything revolves around the interface provided to the user.



## Abstraction

The ability to hide the complexity of an object behind a simple interface.

An abstraction emphasises the features and behaviour of the object.

#### Examples

- Algorithms and Functions
- Types and Classes
- Modules and Namespaces

#### Basically

The Black Box.

Apples go in, apple juice comes out.

### When creating an abstraction

Label an opaque region of space as an object which represents something, and can be used to do a specific task.

The object should provide a simple interface to the user, which is not cluttered with unnecessary implementation details.



## Encapsulation

The ability to prevent internal damage caused by external messages (method calls).

An object with encapsulation ensures that any messages the object processes will leave the object in a valid state.

#### Examples

- Access permissions (private, public)
- Assertions (preconditions, post-conditions, sanity assertions)
- Exception safety

#### Basically

Validation and safety.

The Roadside Barrier. If you drive off the road, you don't fall off the cliff.

### When encapsulating state in an object

Provide a set of operations for users to achieve the desired outcome.

The operations can read (and sometimes mutate) the internal state of the object.

The interface handles user errors and prevents the user from damaging the internal state of the abstraction.

Ideally, the interface never creates observable side effects outside of the object (except for the visitor pattern).



## Polymorphism

The ability to use one interface (way of interacting) with objects of different types.

#### Examples

- Dynamic Polymorphism (Pointer to Base Class)
- Traits
- Overloaded Functions and Operators
- Templates (Generics)
- Concepts and type constraints (Metaprogramming)

#### Basically

The "is a" relationship.

An apple is a fruit. A grape is also a fruit. All fruit come with seeds, therefore both apples and grapes come with seeds.

### When creating a flexible object

Given a set of abstractions, declare that a part of each one has a similar goal/behaviour.

Different abstractions will use different algorithms, but the observable result will be similar.

For a specific operation, it does not matter if the user uses one abstraction over another; they use it in the same way.



## Inheritance

The ability to use another abstraction's interface and properties as part of a new object.

#### Examples

- Class Inheritance:
	- A Derived class inherits the methods, fields, and types of a base class.
- Hierarchical Inheritance:
	- Given a set of complete objects, describe them as leaf nodes in a tree of shared traits.
	- A parent node labels the traits (operations) which are common between its child nodes.
- Composition:
	- An object is composed of smaller objects, and provides operations which use the functionality of the nested objects.



## Notes

Hierarchical inheritance is easily tied to dynamic polymorphism, with v-tables.
