#include <cassert>

static int seeds() {
	return 1;
}
int apples();

int main() {
	assert(3 == apples());
	assert(1 == seeds());
}