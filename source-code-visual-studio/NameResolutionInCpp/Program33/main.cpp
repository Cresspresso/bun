#include <cassert>

struct vec3
{
    union { float x; float r; };
    union { float y; float g; };
    union { float z; float b; };
};
static_assert(sizeof(vec3) == 3 * sizeof(float), "");

int main()
{
    vec3 v;
    v.x = 3.0f;
    assert(3.0f == v.r);
}