#include <condition_variable>
#include <mutex>
#include <thread>
#include <iostream>
#include <future>

std::condition_variable cv;
std::mutex mu;
bool flag(false);

void reacting_func()
{
    std::cout << "waiting...\n";
    std::unique_lock<std::mutex> u_locker(mu);
    // this wait statement fails to account for spurious wakeups
    //cv.wait(u_locker);
    // a task in a wait call is trully blocked.
    cv.wait(u_locker, [](){return flag;});
    std::cout << "start working...\n";
}


void detecting_func()
{
    using namespace std::literals;
    std::thread react_thread(reacting_func);
    std::this_thread::sleep_for(5s);
    {
        std::lock_guard<std::mutex> locker(mu);
        flag = true;
    }
    cv.notify_one();
    react_thread.join();
}


void reacting_func2()
{
    std::cout << "start working...\n";
}

void detecting_func2()
{
    std::promise<void> p;
    std::thread react_thread([&p]{
        // suspend t until future is set
        p.get_future().wait();
        reacting_func2();
    });
    p.set_value();
    react_thread.join();
}


int main()
{
    std::thread t1(detecting_func);
    std::thread t2(detecting_func2);
    t1.join();
    t2.join();
}

