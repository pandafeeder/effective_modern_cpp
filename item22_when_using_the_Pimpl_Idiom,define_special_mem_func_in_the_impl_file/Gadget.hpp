#ifndef __GADGET__H__
#define __GADGET__H__

class Gadget {
public:
    Gadget() = default;
    Gadget(int number)
        : _number(number)
    {}
private:
    int _number { 0 };
};

#endif // __GADGET__H__
