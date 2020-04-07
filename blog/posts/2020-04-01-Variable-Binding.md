
[Previous](./2020-03-10-Language-Theory.md) • [All Posts](../posts.md) • [Next](./2020-04-06-Lexer.md)

1 April 2020

Elijah Shadbolt

# Variable Binding

## Variable Binding in Lambda Calculus

```
λy.yx
```

y is a [bound variable](http://www.cs.columbia.edu/~aho/cs4115/Lectures/15-04-13.html). y is bound to the first lambda.

x is a free variable. It is not bound within this expression.

```
λx.λy.λx.x
```

The final x is bound to the third lambda.

Using alpha conversion, this expression is equivalent to:

```
λx.λy.λz.z
```

## Identifiers in C++

### Block Scopes

```cpp
int x() { return 2; }

int main()
{
	int x = 1;
	int y = 3;
	assert(x == 1);
	assert(y == 3);
}
```

The local variable x hides the global function x.

Local names hide names in parent scopes.

```cpp
char const*const x = "hello world";

int main()
{
	int x = 1;
	{
		char x = 'a';
		assert(x == 'a');
	}
	assert(x == 1);
}
```

Variables in different scopes can have different types.

### Overload Sets

```cpp
int x() { return 0; }
int x(int y) { return y; }

int main()
{
	assert(x() == 0);
	assert(x(10) == 10);
}
```

Functions can be overloaded. A function name is actually an overload set. The overload is resolved based on the argument types of the function call expression; the function name followed by parenthesised comma-separated list of argument expressions.

### Argument Dependent Lookup

```cpp
namespace big
{
	struct Alpha {};
	struct Beta {};
	int x(Alpha) { return 0; }
	int x(Beta) { return 1; }
}

namespace small
{
	int x(int) { return 2; }
}

int main()
{
	assert(x(big::Alpha()) == 0);
	assert(x(big::Beta()) == 1);
	assert(small::x(100) == 2);
}
```

Argument Dependent Lookup means an overload for function x can be resolved for type Alpha because it has a function named x in the namespace that Alpha was declared in, which takes Alpha as an argument. Thus, the function x does not have to be brought into scope manually to have it resolved. However, ADL does not kick in for built-in types or types declared in other namespaces.

### Namespace Pollution

```cpp
#include <cmath>
#include <cassert>
int main()
{
	assert(cos(3.14) < -0.9);
}
```

The global namespace is polluted with things carried over from C libraries.

```cpp
// epsilon.h

#ifndef HEADER_EPSILON_H
#define HEADER_EPSILON_H

namespace alpha
{
	namespace beta
	{
		namespace gamma
		{
			namespace delta
			{
				struct epsilon {};
			}
		}
	}
}

#endif // HEADER

// zeta.h

#ifndef HEADER_ZETA_H
#define HEADER_ZETA_H

#include "epsilon.h"

namespace zeta
{
	int Zeta(alpha::beta::gamma::delta::epsilon) { return 6; }
}

#endif // HEADER

// main.cpp

#include "zeta.h"

using namespace alpha::beta::gamma::delta;
using namespace zeta;

int main()
{
	assert(Zeta(epsilon()) == 6);
}
```

The header file zeta.h must use verbose fully-qualified name of epsilon in order to avoid name pollution. This makes it more explicit for users of the header file, but it takes longer for the programmers to write. It would be great if we had a way to import aliases for a local block scope. Example shown below.

```cpp
// zeta.h

#ifndef HEADER_ZETA_H
#define HEADER_ZETA_H

#include "epsilon.h"

// fake c++
using namespace alpha::beta::gamma::delta
{
	namespace zeta
	{
		int Zeta(epsilon) { return 6; }
	}
}

#endif // HEADER
```

### Member Function Hiding

```cpp
class Alpha
{
public:
	int x(int a) { return a; }
};

class Beta : public Alpha
{
public:
	std::string x() { return "beta"; }
};

int main()
{
	auto b = Beta();
	assert(b.x() == "beta");
	//assert(b.x(2) == 2); // this line will not compile
}
```

Member function Alpha::x(int) is hidden due to Beta::x().

```cpp
class Alpha
{
public:
	virtual int x(int a) { return 1; }
};

class Beta : public Alpha
{
public:
	int x(long a) { return 2; }
};

int main()
{
	auto b = Beta();
	assert(b.x(0) == 2); // appears to work fine.
	Alpha* a = &b;
	assert(a->x(0) == 1); // virtual function x is not overloaded in Beta.
}
```

Slight difference in function signature `int Beta::x(long)` means `int Alpha::x(int)` is hidden instead of overridden. Easy to miss error, cause for concern. This is why using the keywords `override` or `final` is encouraged.

## Identifiers in Rust

Rust does not allow function overloading.

Rust takes a long time to compile, due to checking lifetimes.

### Trait Methods

```rust
trait Alpha
{
	fn x() -> &'static str;
	fn y() -> i32;
}
trait Beta
{
	fn x() -> i32;
}

struct Data {}
impl Alpha for Data
{
	fn x() -> &'static str { return "Alpha"; }
	fn y() -> i32 { return 1; }
}
impl Beta for Data
{
	fn x() -> i32 { return 2; }
}

fn main()
{
	let a = Data{};
	assert!(a.y() == 1);
	assert!(a.Alpha::x() == "Alpha");
	assert!(a.Beta::x() == 2);
}

```

Traits can have methods with the same name. If both traits are in scope, the function call must be [disambiguated](https://doc.rust-lang.org/rust-by-example/trait/disambiguating.html).

## Identifiers in Scala

Scala's handling of identifiers, operators, function calls, and implicit variables will cause very long compilation times.

There can be many implicitly added elements which may be unwanted or unexpected.

### Unicode Identifiers

Many combinations of symbol and letter characters can form an identifier. Custom operators can have user-declared identifiers.

### Method Invocation

[Methods](https://docs.scala-lang.org/style/method-invocation.html) can be invoked as usual `a.+(b)` or with infix notation `a + b`. Binary operators are just methods.

Method names can be decorated for additional functionality, like prefix operators `unary_-` and property assignment methods (setters) `name_=`.

### Implicits

A confusing mess for beginners and even experienced programmers.
