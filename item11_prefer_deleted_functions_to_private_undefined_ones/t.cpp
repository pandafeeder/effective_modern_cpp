

// the difference between deleting fuctions and declaring them private
// my seem more a matter of fashion than anything else, but there's
// greater substance here than you might think. Deleted functions may
// not be used in any way, so even code that's in the member and friend
// functions will fail to compile if it tries. That's an improvement
// over the C++98 behavior, where such improper usage wouldnt be diagnosed
// until link-time.
//
// By convention, deleted functions are declared public, not private.
// compile will check accessibility before deleted status. making delete
// function public will generally result in better error message.
//
// An important advantage of deleted functions is that any function my
// be deleted.

bool isLucky(int number);
bool isLucky(char) = delete;   //reject chars
bool isLucky(bool) = delete;   //reject bool
bool isLucky(double) = delete; //reject doubles and floats


// another trick that deleted functions can perform is to prevent use of
// template instantiations that should be disabled.

template<typename T>
void processPointer(T* ptr);

template<>
void processPointer<void>(void*) = delete;

template<>
void processPointer<const void>(const void*) = delete;

template<>
void processPointer<char>(char*) = delete;

template<>
void processPointer<const char>(const char*) = delete;

// if you have a function template inside a class and you'd like to disable
// some instantiations by declaring them private, you can't, because it's
// not possible to give a member function template specialization a different
// access level from that of the main template.
// The problem is that template specializations must be written at namespace
// scope, not class scope.

/*
class Widget {
public:
    template<typename T>
    void processPointer(T* ptr);
private:
    template<>
    void processPointer<void>(void*);
}
*/

// This issue doesn't arise for deleted functions, because they dont' need a 
// different access level. They can be deleted outside the class.

class Widget {
public:
    template<typename T>
    void processPointer(T* ptr);
};

template<>
void Widget::processPointer<void>(void*) = delete;


int main()
{}
