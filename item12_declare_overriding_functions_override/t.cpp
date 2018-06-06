#include <iostream>
#include <memory>
#include <vector>

// for overriding to occur, several requirements must be met:
// 1. The base class function must be virtual.
// 2. The base and derived function names must be identical(
//    except in the case of destructors).
// 3. The parameter types of the base and derived functions must
//    be identical.
// 4. The constness of the base and derived functions must be identical.
// 5. The return types and exception specifications of the base and
//    derived functions must be compatible.
// 6. C++11 adds one more:
//    The functions' reference qualifiers must be identical.
//    They make it possible to limit use of a member function to lvalues
//    only or to rvalues only. Member functions needs not be virtual to
//    use them.

class Base {
public:
    virtual void doWork()
    {
        std::cout << "Base doWork\n";
    }
};

class Derived: public Base {
public:
    virtual void doWork() override
    {
        std::cout << "Derived doWork\n";
    }
};

class Widget {
public:
    using DataType = std::vector<double>;
    void doWork() &  // this version applies only when *this is an lvalue
    {
        std::cout << "lvalue doWork\n";
    }
    void doWork() && // this version applies only when *this is an rvalue
    {
        std::cout << "rvalue doWork\n";
    }
    DataType& data() &
    {
        return values;
    }
    DataType data() &&
    {
        return std::move(values);
    }
private:
    DataType values;
};

Widget makeWidget()  //factory returns rvalue
{
    return Widget();
}


int main()
{
    std::unique_ptr<Base> upb = std::make_unique<Derived>();
    upb->doWork();

    Widget w;
    w.doWork();
    makeWidget().doWork();

}
