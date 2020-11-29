#include <cassert>
int main()
{
    int originalVariable = 3;
    int& referenceVariable = originalVariable;
    referenceVariable += 4;
    assert(7 == originalVariable);
}