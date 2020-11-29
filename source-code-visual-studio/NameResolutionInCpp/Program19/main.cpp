#include <cassert>

struct Vec2 {
    float x, y;

    // Overload addition-then-assignment operator.
    Vec2& operator+=(Vec2 const& rightHandSide) {
        x += rightHandSide.x;
        y += rightHandSide.y;
        return *this;
    }
    // Overload subtraction-then-assignment operator.
    Vec2& operator-=(Vec2 const& b) {
        x -= b.x;
        y -= b.y;
        return *this;
    }

    // Overload equal oeprator.
    bool operator==(Vec2 const& rightHandSide) const {
        return x == rightHandSide.x
            && y == rightHandSide.y;
    }
    // Overload not equal operator.
    bool operator!=(Vec2 const& b) const {
        return !(*this == b);
    }
};

// Overload addition operator.
Vec2 operator+(Vec2 leftHandSide, Vec2 const& rightHandSide) {
    leftHandSide += rightHandSide;
    return leftHandSide;
}
// Overload subtraction operator.
Vec2 operator-(Vec2 a, Vec2 const& b) {
    a -= b;
    return a;
}

int main() {
    Vec2 const alpha = Vec2{ 3, 4 };
    Vec2 const beta = Vec2{ 0, 1 };
    assert(alpha != beta);

    Vec2 const gamma = Vec2{ 3, 3 };
    assert(beta + gamma == alpha);

    Vec2 delta = { 3, 4 };
    assert(delta == alpha && delta != beta);
    delta -= gamma;
    assert(delta != alpha && delta == beta);
}