#include <vector>
#include <string>
#include <iterator>
#include <algorithm>
#include <iostream>
#include <list>
#include <regex>

class Widget {
public:
    Widget() = default;
    Widget(int i)
        : number(i)
    {}
private:
    int number { 0 };
};

void killWidget(Widget* pWidget)
{
    std::cout << "Widget deleted\n";
    delete pWidget;
}

int main()
{
    std::vector<std::string> vs;
    // "xyzzy" isn't a std::string, the compiler see a mimstach
    // between the type of the argument(const char[6]) and the 
    // type of the parameter taken by push_back (a reference to a std::string).
    // They address the mismatch by generating code to create a temporary
    // std::string object from the string literal, and they pass that
    // temporary object to push_back.
    // So below code makes 2 string constructor call and destructor
    // 1. string constructor for temp std::string("xyzzy")
    // 2. move constructor
    // 3. after push_back returns, temp is destroyed, calling string destructor
    vs.push_back("xyzzy");

    // construct std::string inside vs directly from "xyzzy"
    vs.emplace_back("xyzzy");
    vs.emplace_back(50, 'x');

    // push_back vs emplace_back
    // push_front vs emplace_front
    // insert vs emplace
    // emplace_hint 
    // std::forward_list -> emplace_after vs insert_after

    std::string queenOfDisco("Donna Summer");
    // below do the same thing, emplacement can do everything insertion can
    // they sometimes do it more efficiently
    vs.push_back(queenOfDisco);
    vs.emplace_back(queenOfDisco);

    // if all the following are true, emplacement will almost certainly output-perform insertion
    // 1. The value being added is constructed into the container, not assigned.
    // 2. The argument type being passed differ from the type held by the container
    // 3. The container is unlikely to reject the new value as a duplicate.

    std::copy(vs.begin(), vs.end(), std::ostream_iterator<std::string>(std::cout, " "));
    std::cout << '\n';

    // when deciding whether to use emplacement functions, two other issues are worth keeping
    // in mind. The first regards resource management.
    std::list<std::shared_ptr<Widget>> ptrs;

    ptrs.push_back(std::shared_ptr<Widget>(new Widget, killWidget));
    ptrs.push_back({new Widget, killWidget});

    // the raw pointer resulting from new Widget is perfect-forwarded to the point inside
    // emplace_back where a list node is to be allocated. That allocation fails, and an out-of-memory
    // exception is thrown. As the exception propagates out of emplace_back, the raw pointer that
    // was the only way to get at the Widget on the heap is lost. That Widget is leaked.
    ptrs.emplace_back(new Widget, killWidget);

    // Fundamentally, the effectiveness of resource-managing classes like std::shared_ptr and 
    // std::unique_ptr is predicated on resources being immediately passed to constructors for 
    // resource-managing objects. The fact that functions like std::make_shared and std::make_unique
    // automate this is one of the resons they're so important.
    // When working with containers of resource-managing objects, you must take care to ensure that
    // if you choose an emplacement function over its insertion counterpart, you're not paying for
    // improved code efficiency with diminished exception safety.
    std::shared_ptr<Widget> spw(new Widget, killWidget);
    ptrs.push_back(std::move(spw));
    std::shared_ptr<Widget> spw2(new Widget, killWidget);
    // emplacement are unlikely to outperform insertion fuctions when you're adding resouce-managment
    // objects to a container and you follow the proper practice of ensuring that nothing can intervene
    // between acquring a resouce and turning it over to a resource-managing object.
    ptrs.emplace_back(std::move(spw));

    // A second noteworthy aspect of emplacement functions is their interaction with explicte constructors.
    std::vector<std::regex> regexes;
    // compiler will accept below
    regexes.emplace_back(nullptr);
    // compiler won't accept below
    //regexes.push_back(nullptr);
    
    // The curious behavior you're experiencing stems from the fact that std::regex objects can be constructed
    // from character string. That what makes useful code like below legal:
    std::regex upperCaseWord("[A-Z]+");
    // Creation of a std::regex from a character string can exact a comparatively large runtime cost. so, to
    // minimize the likelihood that such an expense will be incurred unintentionally, the std::regex constructor
    // taking a const char* pointer is explicit. That's why below lines won't compile:
    // std::regex r = nullptr;
    // regexes.push_back(nullptr);
    // In the call to emplace_back, we're passing a constructor argument for a std::regex object.
    // That's not considered an implicit conversion request. It's viewed as if you'd written this code:
    // std::regex r(nullptr);   this compiles
}
