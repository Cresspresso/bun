// This file gets compiled in one translation unit.
#include <cassert>
int get_three() { return 3; }
void check_four(int x)
{
    assert(4 == x);
}