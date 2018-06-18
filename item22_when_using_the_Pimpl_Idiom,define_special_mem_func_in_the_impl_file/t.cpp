#include "Widget.hpp"
#include <iostream>

// The Pimpl Idiom is a way to reduce compilation dependencies
// between a class's implementation and the class's clients,
// but conceptually, use of the idiom doesn't change what the 
// class represent.

int main()
{
    Widget w;
    w.say();
}
