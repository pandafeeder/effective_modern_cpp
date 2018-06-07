#include <string>


// Whether a function should be declared as noexcept is a matter of interface design.
// Callers can query a function's noexcept status, and the results of such a query can
// affect the exception safety or efficiency of the calling code. Failure to declare a
// function noexcept when you know that it won't emit an exception is simply poor
// interface specification.
// But there's an additional incentive to apply noexcept to functions that won't produce
// exceptions: it permits compilers to generate better object code.


int f1(int x) throw();  // no exceptions from f: C++98 style

// in a noexcept function, optimizers need not keep the runtime stack in an unwindable state
// if an exception would propagate out of the function, nor must they ensure that objects in
// a noexcept function are destroyed in the inverse order of construction should an exception
// leave the function. functions with "throw()" exception specifications lack such optimization
// flewsibility, as do functions with no exception specification at all.
int f2(int x) noexcept; // no exceptions from f: C++11 style


// The fact that swapping higher-level data structures can generally be noexcept only if 
// swapping their low-level constituents is noexcpet should motivate you to offer noexcept
// swap functions whenever you can.

template<class T, std::size_t N>
void swap(T (&a)[N], T (&b)[N]) noexcept(noexcept(swap(*a, *b)));

template<class T1, class T2>
struct pair {
    void swap(pair& p) noexcept(noexcept(swap(first, p.first)) &&
                                noexcept(swap(second, p.second)));

    T1 first;
    T2 second;
};


// By default, all memory deallocation functions and all destructors - both user-defined and
// compiler-generated are implicitly noexcept. There's thus no need to declare them noexcept.
// (doing so doesn't hurt anything, it's just unconventional)
int main()
{
}
