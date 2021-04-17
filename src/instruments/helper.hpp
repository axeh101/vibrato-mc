#ifndef HELPER_HPP
#define HELPER_HPP

enum  ExerciceType {European, American};
enum  OptionType {Call=1, Put=-1};

template<typename T=double>
class OptionGreeks
{
    T delta;
    T gamma;
    T vega;
    T theta;
    T rho;
    T vanna;
    T volga;
};

template <typename T=double>
class OptionPremium {
    T premium;
};

template <typename T=double>
class PricingResults {
    OptionPremium<T>& premium;
    OptionGreeks<T>& greeks;
};

#endif  // HELPER_HPP
