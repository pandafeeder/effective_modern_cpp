#include <iostream>
#include <algorithm>
#include <iterator>
#include <memory>
#include <utility>
#include <vector>
#include <functional>

class Widget {
public:
    Widget(int i): number(i) {};
    bool isValidated()
    {
        return number >= 10;
    }
    bool isProcessed()
    {
        return number >= 100;
    }
    bool isArchived()
    {
        return number >= 1000;
    }
private:
        int number;
};

// a lambda expression is simply a way to cause a class to be
// generated and an object of that to be created. There's nothing 
// you can do with a lambda that you can't do by hand.

class isValAndArch {
public:
    using DataType = std::unique_ptr<Widget>;
    explicit isValAndArch(DataType&& ptr)
        : pw(std::move(ptr)) {}

    bool operator()() const
    {
        return pw->isValidated() && pw->isArchived();
    }
private:
    DataType pw;
};

int main()
{
    auto pw = std::make_unique<Widget>(100);

    // C++14, init capture, left pw is in closure scope
    // right pw is in the same scope as closure is declared.
    auto func_cpp14_1 = [pw = std::move(pw)] {
        return pw->isValidated() && pw->isArchived();
    };
    std::cout << std::boolalpha
              << func_cpp14_1()
              << '\n'
              << std::noboolalpha;

    auto func_cpp14_2 = [pw = std::make_unique<Widget>(1000)] {
        return pw->isValidated() && pw->isArchived();
    };

    std::cout << std::boolalpha
              << func_cpp14_2()
              << '\n'
              << std::noboolalpha;

    auto functor = isValAndArch(std::make_unique<Widget>(10000));
    std::cout << std::boolalpha
              << functor()
              << '\n'
              << std::noboolalpha;

    
    std::vector<int> vec {1,2,3,4,5};
    // again, C++14 init capture
    [vec = std::move(vec)] {
        std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, " "));
    }();
    std::cout << '\n';

    std::vector<int> vec2 {5,6,7,8,9};
    // it's not possible to move construct an object into a C++11 closure, but it's possible
    // to move construct an object into a C++11 bind object.
    // C++11 work around to move variable into closure
    std::bind([](const std::vector<int>& vec)
        {
            std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, " "));
        },
        std::move(vec2)
    )();
    std::cout << '\n';
}
