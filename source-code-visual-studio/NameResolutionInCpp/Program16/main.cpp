
// This function's name is mangled to
//  ?apples@@YAHHK@Z
//  in MSVC.
int apples(int, unsigned long) { return 3; }

extern "C" {
    // This function's name is not mangled.
    // Its name is `func`, and it can be
    //  declared and used in a C program.
    int func(int i) { return i + 1; }

    void run(void);
};

int main() { run(); }
