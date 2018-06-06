#include <unordered_map>
#include <memory>
#include <vector>
#include <string>
#include <list>

typedef std::unique_ptr<std::unordered_map<std::string, std::string>> UptrMapSS;
using UptrMapSS2 = std::unique_ptr<std::unordered_map<std::string,std::string>>;

// many people find the alias declaration easier to swallow when dealing with
// types involving function pointers:
typedef void (*FP)(int, const std::string&);
using FP2 = void (*)(int, const std::string&);


// a compelling reason does exist: templates for choosing alias declarations over typedefs
// they are call 'alias templates' while typedef cannot.
template<typename T>
struct MyAlloc {
    using value_type = T;
    MyAlloc() = default;
};

// with typedef, you have to do:
template<typename T>
struct MyAllocList {
    typedef std::list<T, MyAlloc<T>> type;
};

template<typename T>
class Widget {
private:
    typename MyAllocList<T>::type list;
};


// with alias declaration, it's easy
template<typename T>
using MyAllocList2 = std::list<T, MyAlloc<T>>;

template<typename T>
class Widget2 {
private:
    MyAllocList2<T> list;
};


int main()
{
}
