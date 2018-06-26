#include <vector>
#include <thread>
#include <iostream>
#include <mutex>
#include <atomic>
#include <cmath>


class Polynomial {
public:
    using RootsType = std::vector<double>;
    // roots declared const, but it's not thread safe.
    //RootsType roots() const
    //{
    //    if (!rootsAreValid) {
    //        rootVals = { 1,2,3,4,5 };
    //        rootsAreValid = true;
    //    }
    //    return rootVals;
    //}
    RootsType roots() const
    {
        std::lock_guard<std::mutex> g(m);
        if (!rootsAreValid) {
            rootVals = { 1,2,3,4,5 };
            rootsAreValid = true;
        }
        return rootVals;
    }
    // a thread calls magicValue, sees cachedValue is false, performs
    // the two expensive computations and assigns their sum to cachedValue.
    // at the same point, another thread call this function, alse sees
    // cachedValid false, thus the same expensive computation run.
    //
    // the lesson here is that for a single variable or memory location 
    // requiring synchronization, use of a std::atomic is adequate, but once
    // you get to two or more variables or memory locations that require manipulation
    // as a unit, you should reach for a mutex.
    int magicValue() const
    {
        if (cacheValid) return cachedValue;
        else {
            // assuming val1, val2 is calculated by expensiveComputation
            auto val1 = 1000;
            auto val2 = 10000;
            cachedValue = val1 + val2;
            cacheValid = true;
            return cachedValue;
        }
    }
private:
    // std::mutex is a move-only type which makes Polynomial
    // loses the ability to be copied.
    mutable std::mutex m;
    mutable bool rootsAreValid { false };
    mutable RootsType rootVals {};
    mutable std::atomic<bool> cacheValid { false };
    mutable std::atomic<int> cachedValue;
};

class Point {
public:
    double distanceFromOrigin() const noexcept
    {
        ++callCount;
        return std::sqrt((x * x) + (y * y));
    }
private:
    // std::atomic is move-only types, so Point is also move-only
    mutable std::atomic<unsigned> callCount { 0 };
    double x, y;
};


int main()
{
    Polynomial p;
    auto r = p.roots();
    for (const auto& i : r) {
        std::cout << i << '\n';
    }
}

