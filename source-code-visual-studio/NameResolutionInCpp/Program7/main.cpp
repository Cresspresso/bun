#include <cassert>
class Basket {
private:
    int apples;
public:
    int get_apples() const {
        return apples;
    }
    // Parameter hides member variable.
    void set_apples(int apples) {
        this->apples = apples;
    }
    // Constructor initializer list
    // already knows the difference.
    explicit Basket(int apples) : apples(apples) {}
};
int main() {
    Basket basket = Basket(1);
    assert(1 == basket.get_apples());
    basket.set_apples(3);
    assert(3 == basket.get_apples());
}