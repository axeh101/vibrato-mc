#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>
#include "../instruments/helper.hpp"
#include "../instruments/option.hpp"
#include "../processes/process.hpp"

template <typename T, class TProcess>
class PricingEngine {

    protected:
        Option<T>& option_;
        TProcess& process_;


    public:
        PricingEngine(Option<T>& option, TProcess& process)
            :option_(option), process_(process){};
        virtual~PricingEngine() = default;

        friend std::ostream & operator<<(std::ostream & o, const PricingEngine<T, TProcess>& engine)  {
            return engine.displayResults(o);
        }
        virtual std::ostream & displayResults(std::ostream& o) const {
            return o << option_ <<std::endl << process_ << std::endl <<
                "Premium: " << this->premium() << std::endl <<
                "Delta: " << this->delta() << std::endl <<
                "Gamma: " << this->gamma() << std::endl <<
                "Vega: " << this->vega() << std::endl <<
                "Vanna: " << this->vanna() << std::endl <<
                "Volga: " << this->volga() << std::endl <<
                "Rho: " << this->rho() << std::endl <<
                "Theta: " << this->theta() << std::endl;
        };

        virtual T rho() const = 0;
        virtual T vega() const = 0;
        virtual T delta() const = 0;
        virtual T gamma() const = 0;
        virtual T theta() const = 0;
        virtual T vanna() const = 0;
        virtual T volga() const = 0;
        virtual T premium() const = 0;
};
#endif  // ENGINE_HPP
