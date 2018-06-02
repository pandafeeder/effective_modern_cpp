#include <iostream>
#include <cstdint>
#include <tuple>
#include <type_traits>

namespace Demo_unscoped {
    // unscoped, enumerator leaked to the same scope 
    // where enum is declared
    // enumerators for unscoped enums implicitly conver to integral types 
    // (and, from there, to floating-point types...)
    enum Color { black, white, red };
    // error! white already declared in this scope
    // static const unsigned char white[3] = {0xFF, 0xFF, 0xFF};

    // error, unscoped enum can't be forward declared
    // enum Forward_declared_enum;
    // to specify the underlying type for an unscoped enum,
    // and the result may be forward declared
    enum Status: std::uint32_t;

    using UserInfo = 
        std::tuple<std::string, // name
                   std::string, // email
                   std::size_t>;  // reputation

    enum UserInfoFields {uiName, uiEmail, UiReputation};

}

namespace Demo_scoped {
    enum class Color { black, white, red };
    // scoped enum has no above issue
    // there are no implicit conversions from enumerators
    // in a scoped enum to any other type
    static const unsigned char white[3] = {0xFF, 0xFF, 0xFF};

    // scoped enum can be forward declared
    enum class Forward_declared_enum;

    // by default, the underlying type for scoped enums is int,
    // you can override it
    enum class Status: std::uint32_t;

    enum class UserInfoFields { uiName, uiEmail, uiReputation };
}

template<typename E>
constexpr
auto
toUType(E enumerator) noexcept
{
    return 
        static_cast<std::underlying_type_t<E>>(enumerator);
}


int main()
{
    auto black_u = Demo_unscoped::black;
    auto black_s = Demo_scoped::Color::black;

    if (black_u < 1.1) {
        std::cout << "unscoped enum was implicitly converted\n";
    }

    if (static_cast<double>(black_s) < 1.1) {
        std::cout << "scoped enum was not implicitly converted\n";
        std::cout << "you can cast it to what you want\n";
    }


    // when referring to field within std::tuple, unscoped enum's enumerator can
    // be used to indicat which field of the tuple represent, that's due to the 
    // implicit conversion from UserInfoFields to std::size_t
    Demo_unscoped::UserInfo user = std::make_tuple("name", "name@demo.com", 11);
    auto user_name = std::get<Demo_unscoped::uiName>(user);
    std::cout << "user name: " << user_name << '\n';

    using namespace Demo_scoped;
    // corresponding code with scoped enums is substantially more verbose
    auto user_mail = std::get<static_cast<std::size_t>(UserInfoFields::uiEmail)>(user);
    std::cout << "user email: " << user_mail << '\n';

    auto user_reputation = std::get<toUType(UserInfoFields::uiReputation)>(user);
    std::cout << "user reputation: " << user_reputation << '\n';

}
