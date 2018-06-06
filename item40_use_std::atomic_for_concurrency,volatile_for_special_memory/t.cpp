#include <atomic>
#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>

// volatile is the way we tell compilers that we're dealing with
// special memory. Its meaning to compilers is "Don't perform any
// optimizations on operations on this memory".


// std::atomic is useful for concurrent programming, but not for 
// accessing special memory.
// std::atomic is for data accessed from multiple threads without
// using mutexes. It's a tool for writing concurrent sofeware.
//
// volatile is useful for accessing special memory, but not for 
// concurrent programming.
// volatile is for memory where reads and writes should not be 
// optimized away. It's a tool for working with sepcial memory.

std::condition_variable cv;
std::mutex mu;

int computeImportantValue(int x)
{
    return x * 100;
}



void detecting_and_reacting()
{
    // declaring valAvailable as std:atomic ensures that our
    // critical ordering requirement
    std::atomic<bool> valAvailable(false);
    int imptValue = 0;
    std::thread t([&valAvailable, &imptValue]{
        std::unique_lock<std::mutex> ulock(mu);
        cv.wait(ulock, [&valAvailable]{return valAvailable == true;});
        std::cout << imptValue << '\n';
        imptValue = 10;
    });
    {
        std::lock_guard<std::mutex> lg(mu);
        imptValue = computeImportantValue(10); 
        valAvailable = true;
    }
    t.join();
    std::cout << imptValue << '\n';
}


int main()
{

    std::atomic<int> ac(0);
    volatile int vc(0);

    std::thread t1([&ac,&vc]{
        ++ac; ++vc;
    });

    std::thread t2([&ac,&vc]{
        ++ac; ++vc;
    });

    t1.join();
    t2.join();
    
    // when both threads have finished, ac's value must be 2
    // because each increment occurs as an indivisible operation.
    std::cout << ac << '\n';
    // vc's value, need to be 2, because its increments may not
    // occur atomically.
    std::cout << vc << '\n';

    detecting_and_reacting();

}
