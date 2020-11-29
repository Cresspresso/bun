#include <cassert>

int originalFunction(int x) {
    return x * 2;
}

int main() {
    int (*functionPointer)(int) = originalFunction;
    assert(4 == functionPointer(2));
    assert(4 == (*functionPointer)(2));

    using fp_type = int(*)(int);
    fp_type const fp = originalFunction;
    assert(4 == fp(2));
}