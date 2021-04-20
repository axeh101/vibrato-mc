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

        T delta_;
        T gamma_;
        T vega_;
        T theta_;
        T rho_;
        T vanna_;
        T volga_;
        T premium_;

    public:
        PricingEngine(Option<T>& option, TProcess& process)
            :option_(option), process_(process){};
        virtual~PricingEngine() = default;
        virtual void calculate() = 0;
        friend std::ostream & operator<<(std::ostream & o, const PricingEngine<T, TProcess>& engine)  {
            return engine.displayResults(o);
        }
        virtual std::ostream & displayResults(std::ostream& o) const {
            return o << option_ <<std::endl << process_ << std::endl <<
                "Premium: " << premium_ << std::endl <<
                "Delta: " << delta_ << std::endl <<
                "Gamma: " << gamma_ << std::endl <<
                "Vega: " << vega_ << std::endl <<
                "Vanna: " << vanna_ << std::endl <<
                "Volga: " << volga_ << std::endl <<
                "Rho: " << rho_<< std::endl <<
                "Theta: " << theta_ << std::endl;
        };

        T rho() const { return rho_; }
        T vega() const { return vega_; }
        T delta() const { return delta_; }
        T gamma() const { return gamma_; }
        T theta() const { return theta_; }
        T vanna() const {return vanna_;}
        T volga() const {return volga_;}
};
#endif  // ENGINE_HPP
