#include <thread>
#include <functional>
#include <vector> 
#include <iterator>
#include <algorithm>
#include <iostream>

// unjoinable thread includes:
// 1. default-constructed std::threads
// 2. std::threads objects that have been moved from
// 3. std::threads that have been joined
// 4. std::threads that have been detached

constexpr auto tenMillion = 1000000;

bool conditionsAreSatisfied(int flag)
{
    using namespace std::literals;
    bool ok = false;
    std::thread t([&ok, flag](){
        std::this_thread::sleep_for(5s);
        ok = flag == 0 ? false : true;
    });
    t.join();
    return ok;
}

void performComputation(const std::vector<int>& vec)
{
    using namespace std::literals;
    std::thread t([&vec](){
        std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, " "));
    });
    t.join();
}

bool doWork(std::function<bool(int)> filter, int maxVal = tenMillion)
{
    std::vector<int> goodVals;

    std::thread t([&filter, maxVal, &goodVals]{
        for (auto i = 0; i <= maxVal; ++i) {
            if (filter(i)) goodVals.push_back(i);
        }
    });

    auto nh = t.native_handle();

    // if conditionsAreSatisfied return true, then the program run well
    // but if it returns false, then t will be destroyed when out of scope
    // without joinning, then the program will be terminated
    // so it's your responsbilityt to ensure that a thread object is made unjoinable
    // on every path out of the scope in which it's defined.
    if (conditionsAreSatisfied(1)) {
        t.join();
        performComputation(goodVals);
        return true;
    }

    return false;
}

// RAII classes are common in the Standard Library. Examples include the STL containers,
// smart pointers, std::fstream objects and many more. And yet there is no standard RAII
// class for std::thread objects, perhaps because the Standardization Committee having
// rejected both join and detach as default options, simply didn't konw what such a class
// shoud do. Fortunately, it's not difficult to write one yourself. For example, the following
// class allows callers to specify whether join or detach should be called when a ThreadRAII 
// object is destroyed:
class ThreadRAII {
public:
    enum class DtorAction { join, detach };

    // because std::thread objects may start runing a function immediately after they are
    // initialized, it's a good habit to declare them last in a class. That guarantees that
    // at the time they are constructed, all the data members that precede them have
    // already been initialized and can therefore be safely accessed by the asynchronously
    // running thread that corresponds to the std::thread data member.
    ThreadRAII(std::thread&& t, DtorAction a)
        : action(a), t(std::move(t)) {}

    // no default move / move assignment operator will be generated when there's user defined
    // destructor, but for this class, the default move / move assignment is usable. So we can
    // explictily ask for that using = default;
    ThreadRAII(ThreadRAII&&) = default;
    ThreadRAII& operator=(ThreadRAII&&) = default;

    ~ThreadRAII()
    {
        if (t.joinable()) {
            if (action == DtorAction::join) {
                t.join();
            } else {
                t.detach();
            }
        }
    }

    std::thread& get() { return t; }

private:
    DtorAction action;
    std::thread t;
};


bool doWork2(std::function<bool(int)> filter, int maxVal = tenMillion)
{
    std::vector<int> goodVals;

    ThreadRAII t(
        std::thread([&filter, maxVal, &goodVals]{
            for (auto i = 0; i <= maxVal; ++i) {
                if (filter(i)) goodVals.push_back(i);
            }
        }),
        ThreadRAII::DtorAction::join  // RAII action
    );

    auto nh = t.get().native_handle();

    if (conditionsAreSatisfied(0)) {
        t.get().join();
        performComputation(goodVals);
        return true;
    }

    return false;
}


int main()
{
    auto filter = [](const auto& i){
        return i % 1000 == 0;
    };
    doWork(filter, tenMillion);
    // even if conditionsAreSatisfied return false, thread is joined before leaving the scope using RAII
    doWork2(filter, tenMillion);
}
