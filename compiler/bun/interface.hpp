#pragma once
/*md

# interface.hpp

Elijah Shadbolt

14 April 2020

## Summary

Declares a public virtual destructor and protected constructors for this class.

## Usage

```cpp
class IThing
{
	BUN_DECLARE_INTERFACE(IThing);
private:
	virtual void VirtualMethod() = 0;
public:
	void Method() { return VirtualMethod(); }
};
```

*/

#define BUN_DECLARE_INTERFACE(Name)\
	public: virtual ~Name() noexcept = default;\
	protected:\
		Name(Name&&) noexcept = default;\
		Name& operator=(Name&&) noexcept = default;\
		Name(Name const&) = default;\
		Name& operator=(Name const&) = default;\
		Name() = default\

#define BUN_INTERFACE_FINAL_DEFAULT(Name)\
	public:\
		~Name() noexcept final = default;\
		Name(Name&&) noexcept = default;\
		Name& operator=(Name&&) noexcept = default;\
		Name(Name const&) = default;\
		Name& operator=(Name const&) = default\
