// This file gets compiled in another translation unit.

// Function prototype with same signature
//  as in the first translation unit.
int get_three();
void check_four(int x);

int main()
{
    check_four(get_three() + 1);
}