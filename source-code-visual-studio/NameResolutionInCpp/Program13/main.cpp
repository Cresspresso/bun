#include <cassert>

// Declare function with signature
// `int my_function(int)`.
int my_function(int alpha);

// We can have multiple declarations
// of the same function signature.
int my_function(int);
// Exact same signature as above.
int my_function(int const beta);

// Forward declared with array parameter type.
int last(int data[], int length);

int main()
{
    assert(2 == my_function(0));

    int data[3]{ 0, 1, 2 };
    assert(2 == last(data, 3));
}

// Implementation (One Definition Rule).
int my_function(int)
{
    return 2;
}

// Different signature (overloaded function).
int my_function(long alpha);

// Incorrect signature (won't compile).
//long my_function(int);

// An array parameter is just a pointer parameter.
// Same signature as forward declaration.
int last(int* data, int length)
{
    return data[length - 1];
}

// `void thing()` and `void thing(void)`
// have the same signature.
void thing();
void thing(void);