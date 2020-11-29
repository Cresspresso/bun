#include <cassert>
namespace market {
    class Basket {
    private:
        int m_apples;
    public:
        // This non-static member function has signature
        //  void ::market::Basket::set_apples(int)
        void set_apples(int value) {
            this->m_apples = value;
            // same as
            //(*this).m_apples = value;
        }

        // This non-static member function has signature
        //  int ::market::Basket::get_apples() const
        int get_apples() const {
            return this->m_apples;
        }
    };
}
int main() {
    using namespace market;

    Basket basket;
    // Use dot operator on the `basket` reference
    //  and call
    //  `void ::market::Basket::set_apples(int)`
    //  on its value.
    basket.set_apples(3);

    Basket const* pBasket = &basket;
    // Use arrow operator on the `pBasket` pointer
    //  and call
    //  `int ::market::Basket::get_apples() const`
    //  on its dereferenced value.
    int apples = pBasket->get_apples();
    assert(3 == apples);
}