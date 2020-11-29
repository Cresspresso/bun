#include <cassert>
class Basket {
private:
    int m_value;
public:
    explicit Basket(int value) : m_value(value) {}

    // Calls resolve to this overload
    //  when used on mutable instances.
    int advance() { return ++m_value; }

    // Calls resolve to this overload
    //  when used on const instances.
    int advance() const { return m_value; }
};
int main() {
    Basket basket = Basket(0);
    assert(1 == basket.advance());
    assert(2 == basket.advance());

    Basket const& cref = basket;
    assert(2 == cref.advance());
    assert(2 == cref.advance());
}