#ifndef __WIDGET__H__
#define __WIDGET__H__

#include <memory>

class Widget {
public:
    Widget();
    ~Widget();
    Widget(const Widget& rhs);
    Widget& operator=(const Widget& rhs);
    Widget(Widget&& rhs);
    Widget& operator=(Widget&& rhs);
    void say() const;
private:
    struct Impl;
    std::unique_ptr<Impl> pImpl;
};

#endif // __WIDGET__H__
