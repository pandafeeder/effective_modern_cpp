#include <iostream>
#include <vector>
#include <functional>
#include <algorithm>

using FilterContainer = std::vector<std::function<bool(int)>>;

void addDivisorFilter(FilterContainer& filters)
{
    auto divisor = 5;
    filters.emplace_back(
        [&](int val){ return val % divisor == 0; }
    );
}

template<typename C>
void workWithContainer(const C& container)
{
    auto divisor = 5;
    using std::begin;
    using std::end;

    if (std::all_of(begin(container), end(container), [&](auto& val){
        return val % divisor == 0;
    })) {
        std::cout << "all 5 modules\n";
    } else {
        std::cout << "not all 5 modules\n";
    }
}

class Widget {
    public:
        Widget(int i): divisor(i) {}
        void add_filter(FilterContainer& filters) const
        {
            // lambda only capture local non-static variable
            // divisor is not, it can be seen due to `this`
            // is captured, this is really bad design
            filters.emplace_back([=](const int& i){
                return i % divisor == 0;
            });
        }
        void say()
        {
            std::cout << "disivor value: " << divisor << '\n';
        }
    private:
        int divisor;
};

int main()
{
    FilterContainer filters;
    // dangling reference
    addDivisorFilter(filters);

    std::vector<int> vec1 {1,2,3,4,5,6,7,8,9,10};
    auto filter_iter = std::find_if(vec1.begin(), vec1.end(), filters[0]);
    if (filter_iter != vec1.end()) {
        std::cout << *filter_iter << '\n';
    }
    workWithContainer(vec1);

    {
        Widget *w = new Widget {3};
        w->add_filter(filters);
        w->say();
        delete w;
    }

    // using g++, this will crash,
    // using clang++, this will not
    std::cout << "filters size: " << filters.size() << '\n';
    auto iter2 = std::find_if(vec1.begin(), vec1.end(), filters[1]);
    if (iter2 != vec1.end()) {
        std::cout << *iter2 << '\n';
    }
}
