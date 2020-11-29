#include <cassert>
#include <cstdint>

int apply(int a, int b) {
    return a + b;
}
// Different parameters, different implementation.
int apply(int a, long b) {
    return a - static_cast<int>(b);
}
// An overload can have a different return type.
char const* apply(char const* p, size_t len) {
    return p + len - 1;
}
// It can even have a different number of parameters.
int apply(int a) {
    return a;
}

// The overload set for `apply` in the global scope
// has four symbols:
//  int apply(int, int)
//  int apply(int, long)
//  char const* apply(char const*, size_t)
//  int apply(int)

int main() {
    // Resolves to `int apply(int, int)`.
    assert(8 == apply(6, 2));

    // Resolves to `int apply(int, long)`.
    assert(4 == apply(6, 2L));

    char const* const str = "hello world";
    // Resolves to `char const* apply(char const*, size_t)`.
    // The `int` argument was implicitly converted to `size_t`.
    assert('d' == *apply(str, 11));

    // Resoves to `int apply(int)`.
    assert(4 == apply(4));
}