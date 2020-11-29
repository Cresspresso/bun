#include <cassert>

int const apples = 0;

class basket
{
    int apples = 1;

public:
    int get_apples() const
    {
        return apples;
        // same as: return this->apples;
    }

    int get_all_apples() const
    {
        // needs explicit scope operator
        // because this->apples is closer in scope.
        return ::apples;
    }
};

int main()
{
    auto b = basket();
    assert(0 == apples);
    assert(1 == b.get_apples());
    assert(0 == b.get_all_apples());
}