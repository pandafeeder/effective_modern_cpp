#include <memory>
#include <iostream>
#include <exception>
#include <vector>

template<typename T, typename... Ts>
std::unique_ptr<T> make_unique(Ts&&... params)
{
    return std::unique_ptr<T>(new T(std::forward<Ts>(params)...));
}

class Widget {
public:
    Widget() = default;
    Widget(int number)
        : _number(number)
    {}
    void say() const
    {
        std::cout << "number: " << _number << '\n';
    }
private:
    int _number { 0 };
};

int computePriority()
{
    throw std::runtime_error("exception");
}

void processWidget(std::shared_ptr<Widget> spw, int priority)
{
    spw->say();
}

int main()
{
    auto upw1(std::make_unique<Widget>());
    upw1->say();
    std::unique_ptr<Widget> upw2(new Widget);
    upw2->say();

    // one allocation for this
    // make_shared allocates a single chunk of memory
    // to hold both the Widget obj and control block
    auto spw1(std::make_shared<Widget>());
    spw1->say();
    // two memory allocation for this:
    // 1. direct use of new
    // 2. allocation for the control block
    std::shared_ptr<Widget> spw2(new Widget);
    spw2->say();

    for (int i = 0; i < 10000; ++i) {
        try {
            // compile may generate code order like below:
            // 1. new Widget to get a raw pointer
            // 2. call computePriority
            // 3. construct shared_ptr with the raw pointer
            // if step 2 throws exception, then there'll be memory leakage
            processWidget(std::shared_ptr<Widget>(new Widget), computePriority());
        } catch (const std::runtime_error& e) {
            std::cout << e.what() << '\n';
        }
    }
    // at runtime, either std::make_shared or computePriority will be called first.
    // if computePriority called first, and it throws an exception, make_shared 
    // won't be invoked. The same is for unique_ptr
    try {
        processWidget(std::make_shared<Widget>(), computePriority());
    } catch (const std::runtime_error& e) {
        std::cout << e.what() << '\n';
    }


    // circumstances where make can't or shouldn't be used
    // 1. none of the make functions permit the specification of custom deleters
    //    but unique_ptr shared_ptr constructor do
    // 2. when you want to use initializer_list to construct a object, because
    //    make perfect_forward their args using (), but initializer_list can't
    //    be perfect forwarded. You must use new directly.
    //    below code won't comiple due to reson 2.
    //auto sp_vec = std::make_shared<std::vector<int>>({1,2,3,4,5});

    std::shared_ptr<std::vector<int>> sp_vec(new std::vector<int>{1,2,3,4,5} );
}
