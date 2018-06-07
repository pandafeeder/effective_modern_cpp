#include <iterator>
#include <algorithm>
#include <vector>
#include <iostream>


template<typename C, typename V>
void findAndInsert(C& container, const V& targetVal, const V& insertVal)
{
    using std::cbegin;
    using std::cend;

    auto it = std::find(cbegin(container), cend(container), targetVal);
    container.insert(it, insertVal);
}

template<typename C>
auto cbegin(const C& container)->decltype(std::begin(container))
{
    return std::begin(container);
}



int main()
{
    std::vector<int> vec { 1,2,3,4,5 };
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';
    findAndInsert(vec, 3, 6);
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<int>(std::cout, " "));
    std::cout << '\n';
}
