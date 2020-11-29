#include <cassert>
#define pi (3.14)
int main()
{
    assert(pi > 3.0 && pi < 4.0);
    // The line above gets pre-processed into:
    assert((3.14) > 3.0 && (3.14) < 4.0);

    // This will not compile:
    //int *pi = nullptr;

    // It would be pre-processed into:
    //int * (3.14) = nullptr;

    // This is why it would be a good idea to
    //  rename `pi` to `MY_LIBRARY_PI`,
    //  or better yet use constexpr.
}