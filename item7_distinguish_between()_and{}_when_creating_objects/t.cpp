#include <iostream>
#include <string>
#include <atomic>
#include <initializer_list>

class Widget {
    public:
        void say()
        {
            std::cout << "X: " << x << " Y: " << y << '\n';
        }
    private:
        int x{ 0 };
        int y = 0;
        // int z(0); //error
};

// in constructor calls, parentheses and braches have the same meaning
// as long as std::initializer_list parameters are not involved
class Widget2 {
    public:
        Widget2() = default;
        Widget2(int n, bool h): number(n), has(h)
        {}
        Widget2(int n, double d): number(n), dou(d)
        {}
        Widget2(double d, bool h): dou(d), has(h)
        {}
        void say()
        {
            std::cout << "number: " << number
                      << " dou: "   << dou
                      << std::boolalpha
                      << " has: "   << has 
                      << std::noboolalpha
                      << '\n';
        }
    private:
        int number { 0 };
        double dou { 0.0 };
        bool has { false };
};


// if, however, one or more constructor declare a parameter of type std::initlializer_list,
// calling using the brached initialization syntax strongly prefer the overloads taking
// std::initializer_list. Strongly. If there's any way for compilers to construe a call using a
// brached initializer to be to a constructor taking a std::initializer_list, compilers will
// employ that interpretation.
class Widget3 {
    public:
        Widget3() = default;
        Widget3(int n, bool h): number(n), has(h)
        {}
        Widget3(int n, double d): number(n), dou(d)
        {}
        Widget3(double d, bool h): dou(d), has(h)
        {}
        Widget3(std::initializer_list<long double> il):l_dou(*(il.end() - 1 ))
        {}
        operator float() const
        {
            return dou;
        }
        void say()
        {
            std::cout << "number: " << number
                      << " dou: "   << dou
                      << " l_dou: " << l_dou
                      << std::boolalpha
                      << " has: "   << has 
                      << std::noboolalpha
                      << '\n';
        }
    private:
        int number { 0 };
        double dou { 0.0 };
        bool has { false };
        long double l_dou { 0.0 };
};

int main()
{
    int x(0);
    int y = 0;
    int z{ 0 };// same as int z = {0};

    std::atomic<int> ai1{ 0 };
    std::atomic<int> ai2(0);

    // uncopyable object may be initialized using {} or (), 
    // but not using "="
    // error!
    // std::atomic<int> ai3 = 0;

    // {} prohibits implicit narrowing conversion among built-in types
    double d1 = 1.0, d2 = 2.2, d3 = 3.0;
    // int sum1{ d1 + d2 + d3 };

    // () ok
    int sum2(d1 + d2 + d3);
    
    // zero argument using () declar a function instead of an object
    //Widget w_1();

    // {} has no such probelm
    Widget w_2{};
    w_2.say();

    // when an auto declared variable has a braced initializer,
    // the type deduced is std::initializer_list
    auto deduced = { 1 };
    std::cout << typeid(deduced).name() << '\n';

    Widget2 w2_1 (1, 1.0);
    w2_1.say();

    Widget2 w2_1_b {1, 1.0};
    w2_1_b.say();

    Widget2 w2_2 (1.0, true);
    w2_2.say();

    Widget2 w2_2_b {1.0, true};
    w2_2_b.say();

    ////////
    Widget3 w3_1(10, true);
    w3_1.say();

    // will call the initializer_list one, although the non-std::initializer_list
    // constructors are better match
    Widget3 w3_2{10 ,true};
    w3_2.say();

    Widget3 w3_1_co_1 (w3_1);
    w3_1_co_1.say();

    // for gcc:
    // Widget3 w3_1_co_2 { w3_1 }
    // compiler will implicte convert w3_2 to float, then to long double, so w3_1_co_2
    // will be {0 0 0 false}
    // clang will not do the convertion
    Widget3 w3_1_co_2 {float(w3_1)};
    w3_1_co_2.say();

    Widget3 w3_1_mv_1 (std::move(w3_1));
    w3_1_mv_1.say();

    // for gcc / clang same as above: gcc will do the implicit conversion, clang will not
    Widget3 w3_1_mv_2 {std::move(float(w3_1))};
    w3_1_mv_2.say();

    // for empty braced initializers, it will call default construction.
    // Empty braces means no arguments, not an empty std::initializer_list
    // if you want to call a std::initializer_list constructor with a empty std::initializer_list,
    // you do it by makeing the empty braces a constructor argument - by putting the empty
    // braces inside the parentheses or braces demarcating what you're passing.
    // Widget w4 ({}) / Widget w4 {{}}

}
