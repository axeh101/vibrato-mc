#ifndef OPTION_HPP
#define OPTION_HPP

#include <iostream>
#include <autodiff/forward.hpp>

using namespace autodiff;

enum OptionType {
    Call = 1, Put = -1
};

template<typename T>
class Option {
public:
    virtual~Option() = default;

    Option(T &maturity, T &strike, const OptionType &type, std::string name)
            : maturity_(maturity), strike_(strike), type_(type), name_(name) {}

    virtual T payoff(T price) const = 0;

    T maturity() const { return maturity_; };

    T strike() const { return strike_; };

    std::string name() const { return name_; };

    const OptionType &type() const { return type_; }

    const double dtype() const { return (double) type_; }

    friend std::ostream &operator<<(std::ostream &o, const Option<T> &opt) {
        return opt.describe(o);
    }

    virtual std::ostream &describe(std::ostream &o) const {
        return o << "Name: " << name_ << std::endl <<
                 "Option Type: " << (type_ == OptionType::Call ? "Call" : "Put") <<
                 std::endl <<
                 "Maturity: " << maturity_ << std::endl <<
                 "Strike: " << this->strike_ << std::endl;
    };

protected:
    T maturity_;
    T strike_;
    const OptionType &type_;
    const std::string name_;
};


#endif  // OPTION_HPP
