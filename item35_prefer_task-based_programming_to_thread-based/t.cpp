#include <future>
#include <thread>
#include <iostream>
#include <exception>

int doAsyncWork(int x)
{
    std::cout << x << std::endl;
    return x;
}

int doAsyncWorkWithException(int x)
{
    throw std::runtime_error("something wrong");
    return x;
}

int main()
{
    //thread-based, no straight forward way to get result value
    std::thread t(doAsyncWork, 10);
    t.join();
    //if exception is thrown in thread, whole program will be terminated
    //by std::terminate, no way to catch it
    /*
    try {
        std::thread t2(doAsyncWorkWithException, 10);
        t2.join();
    } catch (...) {
        //no way to catch it
    }
    */

    //task-based
    auto fut = std::async(doAsyncWork, 10);
    auto ret = fut.get();
    std::cout << "get result from future: " << ret << '\n';
    try {
        auto fut2 = std::async(doAsyncWorkWithException, 100);
        auto ret2 = fut2.get();
        std::cout << "get result from future: " << ret2 << '\n';
    } catch (const std::runtime_error& except) {
        std::cout << except.what() << '\n';
    }
}
