#include <cassert>

namespace fruit
{
    struct Apple {
        int m_seeds = 0;
    };

    int get_seeds(Apple const& apple) {
        return apple.m_seeds;
    }

    int add(int a, Apple const& apple, int b) {
        return a + apple.m_seeds + b;
    }
}

int main() {
    fruit::Apple apple = fruit::Apple();
    apple.m_seeds = 5;

    // The compiler must resolve `get_seeds`
    // with argument of type `fruit::Apple&`.
    // ADL detects it should use
    // `int fruit::get_seeds(fruit::Apple const&)`.
    assert(5 == get_seeds(apple));

    // ADL works with multiple parameters.
    assert(7 == add(1, apple, 1));
}