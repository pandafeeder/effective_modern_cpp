#include <memory>
#include <iostream>
#include <exception>
#include <set>
#include <unordered_map>

class Widget {
public:
    Widget() = default;
    Widget(int id)
        : _id(id)
    {}
    void say() const
    {
        std::cout << "Widget " << _id << '\n';
    }
    bool operator< (const Widget& other) const
    {
        return _id < other._id;
    }
    ~Widget()
    {
        std::cout << "Widget " <<  _id << " destroyed\n";
    }
private:
    int _id { 0 };
};


std::shared_ptr<const Widget> loadWidget(int id)
{
    auto sp = std::make_shared<const Widget>(id);
    return sp;
}

std::shared_ptr<const Widget> fastLoadWidget(int id)
{
    // store weak_ptr in container, thus when container is gone,
    // ptr stored in container will be gone along with it
    static std::unordered_map<int, std::weak_ptr<const Widget>> cache;
    auto objPtr = cache[id].lock();
    
    if (!objPtr) {
        objPtr = loadWidget(id);
        cache[id] = objPtr;
    }

    return objPtr;
}

int main()
{
    auto spw = std::make_shared<Widget>(10000);
    std::weak_ptr<Widget> wpw(spw);
    spw = nullptr;
    if (wpw.expired())
        std::cout << "obj already gone, dangling ptr...\n";

    if (auto spw_from_wp = wpw.lock())
        spw_from_wp->say();

    try {
        std::shared_ptr<Widget> spw_from_wp2 (wpw);
    } catch (const std::bad_weak_ptr& e ) {
        std::cout << e.what() << '\n';
    }

    // there should be only 1 dtor called, proving only 1 obj created
    auto sp_w1 = fastLoadWidget(1);
    auto sp_w1_a = fastLoadWidget(1);
    auto sp_w1_b = fastLoadWidget(1);

}
