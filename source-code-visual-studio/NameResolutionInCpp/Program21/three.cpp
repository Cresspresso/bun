// This file gets compiled in one translation unit.
#include <cassert>
#include "three.h"
// Implement the functions which were
//  declared in the header.
int get_three() { return 3; }
void check_four(int x) { assert(4 == x); }
