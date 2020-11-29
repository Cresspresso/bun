#ifndef TIMERS_H
#define TIMERS_H

#include <chrono>

namespace timers
{
    // The fully qualified names are required
    // to avoid polluting this namespace
    // with symbols from other libraries.
    inline std::chrono::duration<float> time_between(
        typename std::chrono::system_clock::time_point const& start,
        typename std::chrono::system_clock::time_point const& end
    )
    {
        // Inside a scope which is not a namespace or a class,
        // we can bring symbols into scope without trouble.
        using namespace std::chrono;
        using D = duration<float>;
        return duration_cast<D>(end - start);
    }
}

#endif // file