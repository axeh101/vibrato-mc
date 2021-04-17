#ifndef OPTION_HPP
#define OPTION_HPP
#include <iostream>
#include "helper.hpp"



template <typename T>
class Option
{
    public:
        virtual~Option() = default;
        Option(double maturity, const OptionType& type)
            :maturity_(maturity), type_(type){}

        virtual T payoff(T price) const =0 ;
        double maturity() const { return maturity_; };

    protected:
            double maturity_;
            const OptionType& type_;
};


#endif  // OPTION_HPP
