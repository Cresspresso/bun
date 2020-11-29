#include <cstdio>
#include "timers.h"
int main() {
    using namespace std::chrono;
    using Clock = system_clock;
    auto const start = Clock::now();

    auto end = Clock::now();
    float total = timers::time_between(start, end).count();
    while (total < 0.2f)
    {
        end = Clock::now();
        total = timers::time_between(start, end).count();
    }

    printf("%f\n", total);
}