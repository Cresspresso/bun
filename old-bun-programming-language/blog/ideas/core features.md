
14 April 2020

Elijah Shadbolt

# Core Features
Chosen core features for Bun

## Scope Members


### Namespaces
```bun
MyNamespace: namespace {
	/* ... */
}
```


### Variables
```bun
namespace MyNamespace
{
	Int myStaticVariable = extern;

	struct MyStruct
	{
		Int myFieldVariable = default;
	}

	Int myFunction(Int myParameter)
	{
		Int myLocalVariable = default;
	}
}
```


### Functions
```bun
namespace MyNamespace
{
	Int myStaticFunction()
	{
		return 3;
	}

	Int another()
	{
		Int localFunction() { return 3; }
		assert(localFunction() == 3);

		mut Int localVariable = 3;
		Int f() { return ^func.localVariable; }
		localVariable = localVariable + 1;
		assert(f() == 4);
	}

	class MyClass
	{
		private Int myData = default;
		public Int myMethod() mutable
		{
			this.myData = 4;
		}
	}
}
```


### Pointers
```bun
namespace MyNamespace
{
	Int myFunction()
	{
		auto buffer = Array<Int, 8>();
		var pointer: *Int = &alpha;
	}
}
```


### Types
```bun
namespace MyNamespace
{
	// multiplicative data type
	struct MyStruct
	{
		Int thing;
		Int alsoAnotherThing;
	}

	// additive data type
	variant MyVariant
	{
		Int apples;
		Int oranges;
	}

	bool isApples(&MyVariant thing)
	{
		return thing.tag == tagOf(thing.apples);
	}
}
```


## Scope Members
- namespaces
- variables
	- static (global)
	- stack (local)
	- field (instance)
- functions
	- static (global)
	- stack (closure)
	- method (instance)
- types
	- additive (union)
	- multiplicative (struct)
	- references
	- moving references
- templates
	- template type arguments `T:$`
	- template value arguments `N:Size`
- namespace aliases
- type aliases
- function aliases

## Primitive Types
- `Void` -> `void` (empty struct)
- `Bool` -> `bool`

- bitset:
	- `BInt` -> `unsigned int`
	- `BShort` -> `unsigned short int`
	- `BLong` -> `unsigned long int`
	- `BLongLong` -> `unsigned long long int`
	- `BChar` -> `unsigned char`
- signed integer arithmetic:
	- `Int` -> `signed int`
	- `Short` -> `signed short`
	- `Long` -> `signed long`
	- `LongLong` -> `signed long long`
	- `SChar` -> `signed char`
- unsigned integer arithmetic:
	- `UInt` -> `unsigned int`
	- `UShort` -> `unsigned short int`
	- `ULong` -> `unsigned long int`
	- `ULongLong` -> `unsigned long long int`
	- `UChar` -> `unsigned char`
- character:
	- `Char` -> `char`
	- `CharUtf8` -> `char8_t`
	- `CharUtf16` -> `char32_t`
	- `CharUtf32` -> `char32_t`
- `Offset` -> `ptrdiff_t`
- `Size` -> `size_t`
- `Pointer<$T>` -> `T*`
- `FunctionPointer<$R, $A...>` -> `R(*)(A...)`
- `Reference<$T>` aka `&<T>` -> `T&`
- `MovingReference<$T>` aka `&&<T>` -> `T&&`
- `Array<$T, N>` -> `T[N]`

## Standard Library
- `allocate Void(Size N)`
- `deallocate Void(Size N)`
