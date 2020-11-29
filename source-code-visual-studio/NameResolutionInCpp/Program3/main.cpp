#include <vector>
#include <cassert>

// Class declaration and definition.
class Apple
{
public:
    // Declare that `Apple` has a
    //  member variable named `x`.
    int x;

    // Declare member function with
    //  signature `int Apple::getX() const`.
    // Also define its implementation inline.
    int getX() const { return x; }

    // Declare a constructor prototype for `Apple`.
    Apple();
};

// Implementation for constructor of my_class.
// Initialises the x member variable.
Apple::Apple() : x() {}

int main()
{
    // Use the typename `Apple`.
    // Call its default constructor.
    Apple instance = Apple();
    instance.x = 3;
    assert(3 == instance.getX());

    // Use the typename `std::vector<int>`.
    std::vector<int> vec{ 3, 4, 5 };
    assert(3 == vec[0]);
}