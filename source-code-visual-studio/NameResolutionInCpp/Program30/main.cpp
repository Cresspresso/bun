#include <type_traits>

int main()
{
    typedef int alias_type;
    static_assert(std::is_same<int, alias_type>::value, "");

    using other = int;
    static_assert(std::is_same<int, other>::value, "");

    alias_type x = 5;
    static_assert(std::is_same<int, decltype(x)>::value, "");
}