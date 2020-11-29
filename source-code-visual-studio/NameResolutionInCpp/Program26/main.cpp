#include <cassert>
class Basket {
private:
    int m_apples;
public:
    explicit Basket(int apples) : m_apples(apples) {}

    // non-static member function.
    int apples() const { return m_apples; }

    // This static member function has signature
    //  int ::market::Basket::max_apples()
    static int max_apples() {
        return 4;
    }

    // This will not compile because of the
    //  non-static apples member function.
    //static int apples() { return 5; }
};
int main() {
    Basket basket = Basket(3);

    // Use scope operator on class type
    //  and call `int ::market::Basket::max_apples()`.
    assert(4 == Basket::max_apples());

    // Use dot operator on class instance
    //  and call static function
    //  `int ::market::Basket::max_apples()`.
    assert(4 == basket.max_apples());
}