#include <cassert>
#include <string>

// Variable with static storage duration.
// Type is `int`.
// Name is `y`.
// Declaration without definition.
extern int y;

int main()
{
    // Variable with local storage duration,
    // stored on the stack.
    // Declaration and definition.
    // Initialised to the value of an empty string.
    std::string x;

    // Variable referenced by name.
    // Assignment operation to modify
    //  the value of the variable.
    x = "peach";

    // Reading the value of x and comparing it.
    assert("peach" == x);
}