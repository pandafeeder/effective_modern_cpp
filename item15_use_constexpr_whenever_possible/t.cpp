#include <iostream>
#include <string>
#include <array>


// Conceptually, constexpr indicates a value that's not only const,
// it's known during compilation.
//
// 1. constexpr objects
//    simply put, all constexpre objects are const, but not all const objects
//    are constexpr. If you want compilers to guarantee that a variable has
//    a value that can be used in contexts requiring compile-time constants, 
//    the tool to reach for is constexpr, not const.
//
// 2. constexpr functions
//    Such functions produce compile-time constants when they are called with
//    compile-time constants. If they're called with values not known until
//    runtime, they produce runtime values.


// In C++11, constexpr functions may contain no more than a single executable
// statements: a return. That sounds more limiting than it is, because two tricks
// can be used to extend the expressiveness of constexpr functions beyond what you
// might think. First, the conditional "?:" operator can be used in place of if-else
// statements, and second, recursion can be sued instead of loops. pow can therefore
// be implemented like this:
constexpr
int pow11(int base, int exp) noexcept
{
    return (exp == 0 ? 1 : base * pow11(base, exp - 1));
}

// In C++14, the restrictions on constexpr functions are substantially looser, so the
// following implementation becomes possible:
constexpr
int pow14(int base, int exp) noexcept
{
    auto result = 1;
    for (int i = 0; i < exp; ++i)
        result *= base;
    return result;
}

// In C++11, all built-in types except void qualify as literal types, but user-defined
// types may be literal too. Because constructors and other member functions may be
// constexpr:

class Point {
public:
    constexpr Point(double xVal = 0, double yVal = 0) noexcept
        : x(xVal), y(yVal)
    {}

    constexpr double xValue() const noexcept { return x; }
    constexpr double yValue() const noexcept { return y; }

    // In C++11, two restrictions prevent Point's member functions setX and setY from
    // being declared constexpr. First, they modify the object they operate on, and in
    // C++11, constexpr member functions are implicitly const. Second, they have void
    // return types, and void isn't a literal type in C++11. Both these restrictions 
    // are lifted in C++14, even Point's setters can be constexpr:
    //void setX(double newX) noexcept { x = newX; }
    //void setY(double newY) noexcept { y = newY; }
    constexpr void setX(double newX) noexcept { x = newX; }
    constexpr void setY(double newY) noexcept { y = newY; }

private:
    double x, y;
};

constexpr
Point midpoint(const Point& p1, const Point& p2) noexcept
{
    return {
        (p1.xValue() + p2.xValue()) / 2,
        (p1.yValue() + p1.yValue()) / 2
    };
}

constexpr Point reflection(const Point& p) noexcept
{
    Point result;
    result.setX(-p.xValue());
    result.setY(-p.yValue());

    return result;
}

int main()
{
    constexpr auto numConds = 5;
    std::array<int, pow11(3, numConds)> results;

    constexpr Point p1(9.4, 27.7);
    constexpr Point p2(28.8, 5.3);
    constexpr auto mid = midpoint(p1, p2);

    constexpr auto reflectMid = reflection(mid);

}
