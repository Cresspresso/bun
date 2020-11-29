#include <cstdio>

// Function prototype (forward declaration).
void my_function(int my_parameter);

int main()
{
    // Function call.
    // Resolves to a specific function symbol
    //  named `my_function` which accepts
    //  one argument of type `int`.
    // This is the call site.
    my_function(5);
}

// Function implementation (definition).
void my_function(int my_parameter)
{
    printf("the number is: %d\n", my_parameter);
}