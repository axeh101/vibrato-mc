#ifndef VANILLAOPTION_HPP
#define VANILLAOPTION_HPP
#include "option.hpp"

template <typename T=double>
class VanillaOption: public Option<T>
{
    public:
        virtual~VanillaOption() = default;
        VanillaOption(double maturity, T strike, const OptionType& type):
            Option<T>(maturity, type), strike_(strike){};

        T payoff(T price) const override
        {
            return std::max(Option<T>::type_ * (price - strike_), 0.0);
        };

        T strike() const { return strike_; };

    protected:
        T strike_;

};

#endif  // VANILLAOPTION_HPP
