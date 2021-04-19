#ifndef VANILLAOPTION_HPP
#define VANILLAOPTION_HPP
#include "option.hpp"

template <typename T=double>
class VanillaOption: public Option<T>
{
    public:
        virtual~VanillaOption() = default;
        VanillaOption(double maturity, T strike, const OptionType& type):
            Option<T>(maturity, strike, type, "Vanilla Option"){};

        T payoff(T price) const override
        {
            return std::max(this->type_ * (price - this->strike_), 0.0);
        };

};

#endif  // VANILLAOPTION_HPP
