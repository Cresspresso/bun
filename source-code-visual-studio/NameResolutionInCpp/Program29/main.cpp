#include <cassert>

class Fruit {
private:
    int m_seeds = int();
public:
    // lifetime rule of 6
    ~Fruit() noexcept = default;
    Fruit(Fruit&&) noexcept = default;
    Fruit& operator=(Fruit&&) noexcept = default;
    Fruit(Fruit const&) = default;
    Fruit& operator=(Fruit const&) = default;
    Fruit() = default;

    explicit Fruit(int seeds) : m_seeds(seeds) {}
    int get_seeds() const { return m_seeds; }
};

class Apple : private Fruit {
public:
    using BaseType = Fruit;

    // Make all constructors of inherited class
    // publicly accessible from this class.
    using Fruit::Fruit;
    using Fruit::operator=;

    // Make all overloads of a
    // privately inherited member function
    // publicly accessible from this class.
    using Fruit::get_seeds;
};

int main() {
    Apple apple = Apple(3);
    assert(3 == apple.get_seeds());
}