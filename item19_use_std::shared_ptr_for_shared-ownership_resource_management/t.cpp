#include <memory>
#include <iostream>
#include <vector>


//1. std::shared_ptr is twice the size of a raw point, one for object raw pointer
//   one for a raw pointer to resource's reference count
//2. Memory for the reference count must be dynamically allocated.
//3. Increments and decrements of the reference count must be atomic


class Widget {
public:
    Widget() = default;
    Widget(int number): _number(number) {};
    void say() const
    {
        std::cout << _number;
    }
private:
    int _number;
};

void makeLogEntry(Widget* pw)
{
    pw->say();
    std::cout << " will be destroyed\n";
}

auto loggingDel = [](Widget *pw) {
    makeLogEntry(pw);
    delete(pw);
};


class Widget2;
std::vector<std::shared_ptr<Widget2>> processWidgets;

class Widget2: public std::enable_shared_from_this<Widget2> {
public:
    // you have to have a shared_ptr to the object first,
    // then use shared_from_this to prevent creating another
    // control block
    static std::shared_ptr<Widget2> create(int number)
    {
        return std::shared_ptr<Widget2>(new Widget2(number));
    }
    void process() {
        std::cout << "process widgets...\n";
        // this will cause a new control block created
        // if there's another shared_ptr already pointing to
        // this obj, then two control blocks free the resource
        // twice.
        // processWidgets.emplace_back(this)
        processWidgets.emplace_back(shared_from_this());
    }
    virtual ~Widget2()
    {
        std::cout << _number << " destroyed\n";
    }
private:
    Widget2(int number): _number(number) {}
    int _number;
};



int main()
{
    std::unique_ptr<Widget, decltype(loggingDel)> upw(new Widget(100), loggingDel);
    // specifying a custom deleter doesn't change the size of a std::shared_ptr object
    // it's actually raw ptr to underlying obj, and prt to dynamic allocated control block
    // an obj's control block is set up by the function creating the first shared_ptr
    // to the obj. the following rules for control block creation are used:
    // 1. make_shared always create a control block. It manufactures a new obj to point to,
    //    so there's certainly no control block for that obj at the time make_shared is called.
    // 2. A control block is created when a shared_ptr is constructed from a unique-ownership pointer.
    // 3. When a shared_ptr constructor is called with a new raw pointer, it creates a control block.
    std::shared_ptr<Widget> spw (new Widget(1000), loggingDel);
    auto wpt = Widget2::create(10000);

}
