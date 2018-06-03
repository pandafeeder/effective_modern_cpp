#include <future>
#include <iostream>
#include <vector>
#include <algorithm>
#include <iterator>
#include <type_traits>

void f()
{
    using namespace std::literals;
    std::this_thread::sleep_for(1s);
}

// making a function that acts like std::async but automatically
// uses std::launch::async as the launch policy, here's C++11 version

template<typename F, typename... Args>
inline
std::future<typename std::result_of<F(Args...)>::type>
realAsync11(F&& f, Args&&... args)
{
    return std::async(std::launch::async,
                      std::forward<F>(f),
                      std::forward<Args>(args)...);
}

// here's C++14 version
template<typename F, typename... Args>
inline
auto
realAsync14(F&& f, Args&&... args)
{
    return std::async(std::launch::async,
                      std::forward<F>(f),
                      std::forward<Args>(args)...);
}

int async_work(const std::vector<int>& vec)
{
    std::cout << "from async_work:\n";
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';
    return *(vec.begin()) + 41; 
}


int main()
{
    using namespace std::literals;
    std::vector<int> vec {1,2,3,4,5};

    // default launch policy is std::launch::async | std::launch::deferred
    auto fut = std::async([&vec](auto i){
        std::for_each(vec.begin(), vec.end(), [&i](auto& v){
            v *= i;
        });
    }, 10);
    fut.get();
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';


    auto f2 = std::async(f);
    // loop until f has finished running... which may never happen
    while (f2.wait_for(100ms) != std::future_status::ready) {
        std::cout << "f2 has not finished running\n";
    }

    auto f3 = std::async(f);
    if (f3.wait_for(0s) == std::future_status::deferred) {
        std::cout << "f3 is arranged synchronously\n"
                  << "use wait or get to call f synchronously";
    } else { // task isn't deferred
        while (f3.wait_for(100ms) != std::future_status::ready) {
            // task is neither deferred nor ready, do concurrent work in this block
            std::cout << "main thread sleeps for 1s\n";
            std::this_thread::sleep_for(1s);
        }
        // f3 is ready
    }

    // launch f4 asynchronously
    auto f4 = std::async(std::launch::async, f);
    while (f4.wait_for(100ms) != std::future_status::ready) {
        std::cout << "main thread sleeps for 5s\n";
        std::this_thread::sleep_for(5s);
    }
    // f4 is ready

    auto f5 = realAsync11(async_work, std::vector<int>{1,2,3,4,5});
    std::cout << f5.get() << '\n';

    auto f6 = realAsync14(async_work, std::vector<int>{1,2,3,4,5});
    std::cout << f6.get() << '\n';
}
