#include <assert.h>
int func(int);
void run(void) {
	assert(2 == func(1));
}