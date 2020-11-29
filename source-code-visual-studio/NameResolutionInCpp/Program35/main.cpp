#include <cassert>
#include <memory>
#include <iostream>

// Class which logs all lifetime semantics.
template<class ValueType>
class LifeLogger {
private:
    ValueType m_value;

public:
    ValueType& ref_value() { return m_value; }
    ValueType const& ref_value() const { return m_value; }
    ValueType copy_value() const { return m_value; }

    ~LifeLogger() noexcept
    {
        std::cout << "~(" << m_value << ")\n";
    }

    LifeLogger(LifeLogger&& b) noexcept
        : m_value(std::move(b.m_value))
    {
        std::cout << "+move(" << m_value << ")\n";
    }

    LifeLogger& operator=(LifeLogger&& b) noexcept
    {
        std::cout << "(" << m_value << ")=move(" << b.m_value << ")\n", m_value;
        m_value = std::move(b.m_value);
        return *this;
    }

    LifeLogger(LifeLogger const& b)
        : m_value(b.m_value)
    {
        std::cout << "+copy(" << m_value << ")\n";
    }

    LifeLogger& operator=(LifeLogger const& b) {
        m_value = b.m_value;
        std::cout << "=copy(" << m_value << ")\n";
        return *this;
    }

    void swap(LifeLogger& b) noexcept
    {
        std::cout << "swap(" << m_value << ", " << b.m_value << ")\n";
        using std::swap;
        swap(m_value, b.m_value);
    }

    LifeLogger() : m_value()
    {
        std::cout << "+()\n";
    }

    explicit LifeLogger(ValueType value)
        : m_value(std::move(value))
    {
        std::cout << "+(" << m_value << ")\n";
    }
};

template<class ValueType>
inline void swap(LifeLogger<ValueType>& a, LifeLogger<ValueType>& b) noexcept
{
    a.swap(b);
}



using Creature = LifeLogger<int>;

Creature originalFunction(Creature x)
{
    std::cout << "originalFunction\n";
    return Creature(x.copy_value() * 2);
}



// Basic forwarding by copying arguments (not moving).
// The extra function call can be optimised away.
inline Creature copyForwardingFunction(Creature x)
{
    return originalFunction(x);
}

// Perfect forwarding of arguments and return type.
template<class T>
decltype(auto) perfectForwardingFunction(T&& x)
{
    return originalFunction(std::forward<T>(x));
}

//  Requires C++17 for automatic type
//   deduction keyword `decltype(auto)`.
auto const perfectForwardingLambda =
[](auto&& x) -> decltype(auto)
{
    return originalFunction(
        std::forward<decltype(x)>(x));
};

// Perfect forwarding for C++11.
template<class T>
decltype(originalFunction(std::declval<T&&>()))
cpp11PerfectForwardingFunction(T&& x)
{
    return originalFunction(std::forward<T>(x));
}



int main()
{
    std::cout << "\n[originalFunction]\n";
    {
        auto const r = originalFunction(Creature(2));
        assert(4 == r.copy_value());
    }

    std::cout << "\n[copyForwardingFunction]\n";
    {
        auto const r = copyForwardingFunction(Creature(2));
        assert(4 == r.copy_value());
    }

    std::cout << "\n[perfectForwardingFunction]\n";
    {
        auto const r = perfectForwardingFunction(Creature(2));
        assert(4 == r.copy_value());
    }

    std::cout << "\n[perfectForwardingLambda]\n";
    {
        auto const r = perfectForwardingLambda(Creature(2));
        assert(4 == r.copy_value());
    }

    std::cout << "\n[cpp11PerfectForwardingFunction]\n";
    {
        auto const r = cpp11PerfectForwardingFunction(Creature(2));
        assert(4 == r.copy_value());
    }
}
