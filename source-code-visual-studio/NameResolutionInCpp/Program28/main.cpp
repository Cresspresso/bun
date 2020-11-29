#include <cassert>

class Fruit {
private:
    // Private members are only accessible in
    // this class (and its member functions).
    int m_skinThickness;

protected:
    // Protected members are only accessible
    // in this class and classes which
    // inherit this class.
    int m_seeds;

public:
    // Public members are accessible
    // from anywhere.
    int get_fruit_seeds() const {
        return m_seeds;
    }
};

// Inherit the Fruit class and make all its
// public members publicly accessible
// from this class.
class Apple : public Fruit {
public:
    // Member functions of a
    // derived class can access
    // protected members.
    int get_apple_seeds() const {
        return m_seeds;
    }
    // The `this` keyword includes
    // inherited properties.
    void set_apple_seeds(int value) {
        this->m_seeds = value;
    }
};

int main() {
    Apple apple = Apple();
    apple.set_apple_seeds(3);
    assert(3 == apple.get_apple_seeds());

    // We can access public properties of
    // inherited classes.
    assert(3 == apple.get_fruit_seeds());

    // Outside the class,
    // we cannot access protected properties.
    //assert(3 == apple.m_seeds);
}