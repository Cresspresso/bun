#include <cassert>

// variable in global scope
int const apples = 0;

int main()
{
    // same name but in nested scope
    int const apples = 1;
    assert(1 == apples);
    if (true)
    {
        int const apples = 2;
        assert(2 == apples);
    }
    assert(1 == apples);
    // fully qualified name to work around name hiding
    assert(0 == ::apples);
}