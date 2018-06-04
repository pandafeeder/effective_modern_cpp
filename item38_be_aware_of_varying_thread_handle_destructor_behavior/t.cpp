#include <future>
#include <thread>
#include <iostream>
#include <vector>

// destruction of a joinable std::thread terminates your program,
// because the two obvious alternatives - an implicit join and
// detach - were considered worse choices. Yet the destructor for
// a future sometimes behaves as if it did an implicti join,
// sometimes as if it did an implicit detach and sometimes neither.
// it never causes program termination.
//
// where's the callee's result stored?
// neither objects associated with the callee nor objects associated
// with the caller are suitable places to store the callee's result.
// it's stored in a location outside both. this location is known as
// the shared state. The shared state is typically represented by a
// heap-based object, but its type, interface, and implementation are
// not specified by the Standard. Standard Library author are free to
// implement shared states in any way the like.


// the normal behavior is that a future's destructor destroys the future
// object. That's it. It doesn't join with anything, it doesn't detach
// from anything. it doesn't run anything. It just destroys the future's
// data members.


// Widget objects might block in their dtors
class Widget {
private:
    std::shared_future<double> fut;
};

int calcVaule()
{
    return 10;
}



int main()
{
    // this container might block in its dtor, because one or more
    // contained futures could refer to a shared state for a non-deferred
    // task launch via std::async.
    std::vector<std::future<void>> futs;

    {
        std::packaged_task<int()> pt(calcVaule);
        auto fut = pt.get_future();
        // std::packaged_tasks are not copyable, so when pt is passed to the
        // std::thread constructor, it must be cast to an rvalue.
        std::thread t(std::move(pt));

        // in the following code:
        // 1. nothing happens to t. In this case, t will be joinable at the
        // end of the scope. That will cause the program to be terminated.
        //
        // 2. A join is done on t. In this case, there would be no need for
        // fut to block in its destructor, because the join is already present
        // in the calling code.
        //
        // 3. A detach is done on t. In this case, there would be no need for
        // fut to detach in its destructor, because the calling code already
        // does that.

        t.join();
        std::cout << fut.get() << '\n';
    }


}
