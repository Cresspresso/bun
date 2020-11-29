#include <cassert>
#include "basket.h"

// When this is expanded by the pre-processor,
//  it includes "basket.h" again
//  in the same translation unit.
// Without header guards,
//  this would cause a linker error.
#include "counter.h"

int main()
{
	auto basket = Basket();
	basket.set_apples(3);
	assert(3 == count_apples(basket));
}