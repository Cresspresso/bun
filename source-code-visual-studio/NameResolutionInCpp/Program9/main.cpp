#include <cassert>

// No implementation here, therefore
//  it must be in another translation unit.
double pi();

int main()
{
	assert(3.1 < pi() && pi() < 3.2);
}