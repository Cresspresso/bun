struct alpha
{
    int originalMemberVariable;
    int& evilVariable = originalMemberVariable;
};
//static_assert(sizeof(alpha) == sizeof(int),
//   "this fails because reference member variables "
//   "are stored as part of the struct");

static_assert(sizeof(alpha) == sizeof(int) + sizeof(int*), "");
int main() {}