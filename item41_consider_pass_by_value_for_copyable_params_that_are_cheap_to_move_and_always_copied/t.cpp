#include <string>
#include <iostream>
#include <vector>
#include <memory>

class Widget {
public:
    /*
    void addName(const std::string& newName)
    {
        names.push_back(newName);
    }
    void addName(std::string&& newName)
    {
        names.push_back(std::move(newName));
    }
    */

    /*
    template<typename T>
    void addName(T&& newName)
    {
        name.push_back(std::forward<T>(newName));
    }
    */

    void addName(std::string newName)
    {
        names.push_back(std::move(newName));
    }
    void say() const noexcept
    {
        std::cout << "names are: ";
        for (const auto& i : names) {
            std::cout << i << " ";
        }
        std::cout << '\n';
        std::cout << "ptr string is: "
                  << *p << '\n';
    }
    
    /* this total cost would thus be two moves
    void setPtr(std::unique_ptr<std::string> ptr)
    { p = std::move(ptr); }
    */

    void setPtr(std::unique_ptr<std::string>&& ptr)
    { p = std::move(ptr); }

private:
    std::vector<std::string> names;
    std::unique_ptr<std::string> p;
};

class Password {
public:
    explicit Password(std::string pwd)
        : text(std::move(pwd)) {}
    // because used copy by value, old passowrd has to be deacclocated,
    // and newPwd will be constructed, if use const std::string& newPwd,
    // and if old pwd is longer than new one, we ca resue text's memory
    // if text.capacity() >= newPwd.size()
    void changeTo(std::string newPwd)
    { 
        text = std::move(newPwd);
    }
private:
    std::string text;
};

int main()
{
    Widget w;
    w.addName("xiaohong");
    w.setPtr(std::make_unique<std::string>("Morden C++"));
    w.say();

    std::string initPwd("Supercalifragilisticexpialidocious");
    Password p(initPwd);

    std::string newPassword = "Beware the Jabberwock";
    p.changeTo(newPassword);

}
