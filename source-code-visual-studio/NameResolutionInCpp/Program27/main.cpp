#include <cassert>
struct Basket {
    int apples = 0;
    int oranges = 0;
    void set_apples(int value) { apples = value; }
    void set_oranges(int value) { oranges = value; }
};
int main() {
    using PFruit = int(Basket::*);
    PFruit pointerToFruit = &Basket::apples;

    using PSetter = void(Basket::*)(int);
    PSetter pointerToSetter = &Basket::set_apples;

    Basket basket;
    Basket* const pBasket = &basket;

    (basket.*pointerToSetter)(3);
    assert(3 == basket.apples);
    assert(0 == basket.oranges);
    assert(3 == (pBasket->*pointerToFruit));

    pointerToFruit = &Basket::oranges;
    pointerToSetter = &Basket::set_oranges;
    (basket.*pointerToSetter)(5);
    assert(3 == basket.apples);
    assert(5 == basket.oranges);
    assert(5 == (pBasket->*pointerToFruit));
}