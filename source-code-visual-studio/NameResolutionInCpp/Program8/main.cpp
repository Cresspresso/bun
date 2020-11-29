namespace alpha
{
    constexpr int apples = 1;

    namespace beta
    {
        constexpr int apples = 2;
        static_assert(2 == apples, "");

        // reach out into a parent namespace
        static_assert(1 == alpha::apples, "");
    }

    static_assert(1 == apples, "");

    namespace gamma
    {
        static_assert(1 == apples, "");

        // reach over into a sibling namespace
        static_assert(2 == beta::apples, "");
    }

    // add members to the namespace beta
    namespace beta
    {
        static_assert(2 == apples, "");
    }
}
// fully qualified name
static_assert(2 == ::alpha::beta::apples, "");
int main() {}