#include <iostream>
#include <functional>


// one of the most exciting features of C++14 is generic lambdas -
// lambdas that use auto in their parameter specifications. The implementation
// of this feature is straight forward: operator() in the lambda's closure class
// is a template.
// given below example

auto multi_by_10 = [](auto x) {return x * 10;};

template<typename T>
class SomeCompilerGeneratedClassName {
public:
    using FuncType = typename std::function<T(T&&)>;
    SomeCompilerGeneratedClassName(FuncType f): func(f) {}
    auto operator()(T&& x) const
    {
        return func(std::forward<T>(x));
    }
private:
    FuncType func;
};


template<typename T>
T dummy(T&& para)
{
    return para * 10;
}

struct Widget {
    int number;
};

void print_Widget(Widget&& w)
{
    std::cout << w.number << '\n';
}

int main()
{
    auto r1 = multi_by_10(10);
    std::cout << "generic lambda returns: " << r1 << '\n';

    // my stupid mimic for compiler generated closure class
    auto my_Functor = SomeCompilerGeneratedClassName<int> (
        dummy<int>
    );
    std::cout << "dummy simulated class returns: " << my_Functor(10) << '\n';

    [](auto&& param){
        std::cout << "generic lambda\n";
        print_Widget(std::forward<decltype(param)>(param));
    }(Widget{100});

}
