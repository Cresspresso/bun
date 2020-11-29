#include <cassert>

namespace three {
    int get_apples() { return 3; }
}

int main() {
    using three::get_apples;
    assert(3 == get_apples());
    static_assert(&get_apples == &three::get_apples, "");
}