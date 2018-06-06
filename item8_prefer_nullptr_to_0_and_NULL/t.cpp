#include <mutex>
#include <memory>
#include <iostream>

// the issue here it that, neither 0 nor NULL has a pointer type.

/*
void f(int);
void f(bool);
void f(void*);

f(0);  //calls f(int), not f(void*)
f(NULL); // might not compile, but typically calls f(int). Never calls f(void*);

this counterintuitive behavior is what led to the guideline for C++98 programmers
to avoid overloading on pointer and integral types. That guideline remains valid 
in C++11, because, the advice of this Item notwithstanding, it's likely that some
developers will continue to use 0 and nULL.

nullptr's advantage is that it doesn't have an integral type.
f(nullptr); //calls f(void*) overload

*/

class Widget
{
public:
    Widget() = default;
    constexpr
    Widget(int c, double p, bool s) noexcept
        : count(c), price(p), sold(s)
    {}
    constexpr int getCount() const noexcept {return count;}
    constexpr double getPrice() const noexcept {return price;}
    constexpr bool getSold() const noexcept {return sold;}
private:
    int count;
    double price;
    bool sold;
};

std::mutex f1m, f2m, f3m;
int f1(std::shared_ptr<Widget> spw)
{
    if (spw)
        return spw->getCount();
    return 0;
}

double f2(std::unique_ptr<Widget> upw)
{
    if (upw)
        return upw->getPrice();
    return 0.0;
}

bool f3(Widget* pw)
{
    if (pw)
        return pw->getSold();
    return false;
}

using MuxGuard  = std::lock_guard<std::mutex>;

template<typename FuncType,
         typename MuxType,
         typename PtrType>
decltype(auto) lockAndCall(FuncType func, MuxType& mutex, PtrType ptr)
{
    MuxGuard g(mutex);
    return func(ptr);
}

int main()
{
    // the following repeated pattern in the calling code is disturbing.
    // use tempalte instead.
    {
        MuxGuard g(f1m);
        auto result = f1(0);
        std::cout << result << '\n';
    }
    {
        MuxGuard g(f2m);
        auto result = f2(NULL);
        std::cout << result << '\n';
    }
    {
        MuxGuard g(f3m);
        auto result = f3(nullptr);
        std::cout << result << '\n';
    }

    // below two won't compile due to deduced type unmatched.
    //auto result1 = lockAndCall(f1, f1m, 0);
    //auto result2 = lockAndCall(f2, f2m, NULL);
    auto result3 = lockAndCall(f3, f3m, nullptr);
    std::cout << result3 << '\n';

}
