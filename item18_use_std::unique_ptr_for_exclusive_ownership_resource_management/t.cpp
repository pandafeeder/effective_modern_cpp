#include <memory>
#include <string>
#include <iostream>
#include <exception>
#include <utility>

class Investment {
public:
    constexpr
    Investment(unsigned int amount, double price) noexcept
        : _amount(amount),
          _price(price),
          _total(amount * price)
    {}

    constexpr
    void sell(unsigned int amount)
    {
        if (_amount < amount)
            throw std::runtime_error("no enough holdings");
        _amount -= amount;
        _total = _amount * _price;
    }

    constexpr
    void buy(unsigned int amount)
    {
        _amount += amount;
        _total = _amount * _price;
    }

    virtual void display() = 0;

    virtual ~Investment() {};
private:
    unsigned int _amount;
    double _price;
    double _total;
};

class Stock : public Investment {
public:
    constexpr
    Stock(unsigned int amount, double price, const char* stock_code) noexcept
        : Investment(amount, price), _stock_code(stock_code) {}
    
    virtual void display() override
    {
        std::cout << "Stock: " << _stock_code;
    }

private:
    const char* _stock_code;
};


class Bond: public Investment {
public:
    constexpr
    Bond(unsigned int amount, double price, const char* bond_name) noexcept
        : Investment(amount, price), _bond_name(bond_name) {}

    virtual void display() override
    {
        std::cout << "Bond: " << _bond_name;
    }
private:
    const char* _bond_name;
};

class RealEstate: public Investment {
public:
    RealEstate(unsigned int amount, double price, const char* address) noexcept
        : Investment(amount, price), _address(address) {}

    virtual void display() override
    {
        std::cout << "RealEstate: " << _address;
    }
private:
    const char* _address;
};

enum class InvestmentType
{
    Stock, Bond, RealEstate
};

void makeLogEntry(Investment* ptr)
{
    ptr->display();
    std::cout << " is deleted\n";
}

auto delInvmt = [](Investment* pInvestment) {
    makeLogEntry(pInvestment);
    delete pInvestment;
};

template<InvestmentType T, typename... Ts>
std::unique_ptr<Investment, decltype(delInvmt)>
makeInvestment(Ts&&... params)
{
    std::unique_ptr<Investment, decltype(delInvmt)> pInv(nullptr, delInvmt);
    switch (T) {
        case InvestmentType::Stock: {
            pInv.reset(new Stock(std::forward<Ts>(params)...));
            break;
        }
        case InvestmentType::Bond: {
            pInv.reset(new Bond(std::forward<Ts>(params)...));
            break;
        }
        case InvestmentType::RealEstate: {
            pInv.reset(new RealEstate(std::forward<Ts>(params)...));
            break;
        }
        default: {
            throw std::runtime_error("unknow investment type");
        }
    }
    return pInv;
}



int main()
{
    auto pInvestment = makeInvestment<InvestmentType::Stock>(100, 192.77, "APPL");
}
