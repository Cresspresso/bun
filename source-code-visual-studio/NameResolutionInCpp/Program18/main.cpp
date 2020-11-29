#include <cassert>

namespace fruit
{
    struct Apple {
        int m_seeds = 0;
    };
}

namespace other
{
    int get_seeds(fruit::Apple const& apple) {
        return apple.m_seeds;
    }
}

int main() {
    fruit::Apple apple = fruit::Apple();
    apple.m_seeds = 5;

    // This will not compile.
    //assert(5 == get_seeds(apple));

    {
        // Instead of ADL, we can import
        // the name into this scope block.
        using other::get_seeds;
        assert(5 == get_seeds(apple));
    }
}