#include <memory>
#include <cassert>

// Forward declare class Apple.
class Apple;

// Use Apple indirectly with a reference type.
int getX(Apple& apple);

// Use Apple indirectly with a pointer type.
std::unique_ptr<Apple> theApple;

// Finish defining the class.
class Apple {
public:
    int x;
};

// Now that the class is defined,
//  we can dereference the reference.
int getX(Apple& apple) { return apple.x; }

int main()
{
    // Perform oprations on instances
    //  of the Apple class.
    theApple = std::make_unique<Apple>();
    theApple->x = 3;
    assert(3 == getX(*theApple));
}