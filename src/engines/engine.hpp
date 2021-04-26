#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>
#include "../instruments/helper.hpp"
#include "../instruments/option.hpp"
#include "../processes/process.hpp"

template<typename T>
class PricingEngine {

protected:
    Option<T> *option_;
    Process<T> *process_;
    T delta_;
    T gamma_;
    T theta_;
    T vega_;
    T rho_;
    T vanna_;
    T volga_;
    T premium_;

public:
    PricingEngine(Option<T> *option, Process<T> *process)
            : option_(option), process_(process) {};

    virtual~PricingEngine() {

        option_ = nullptr;
        process_ = nullptr;
        delete option_;
        delete process_;

    };

    friend std::ostream &operator<<(std::ostream &o, const PricingEngine<T> &engine) {
        return engine.displayResults(o);
    }

    virtual std::ostream &displayResults(std::ostream &o) const {
        return o << *option_ << std::endl << *process_ << std::endl <<
                 "Premium: " << premium_ << std::endl <<
                 "Delta: " << delta_ << std::endl <<
                 "Gamma: " << gamma_ << std::endl <<
                 "Vega: " << vega_ << std::endl <<
                 "Vanna: " << vanna_ << std::endl <<
                 "Volga: " << volga_ << std::endl <<
                 "Rho: " << rho_ << std::endl <<
                 "Theta: " << theta_ << std::endl;
    };

    virtual T rho() { return rho_; };

    virtual T vega() { return vega_; };

    virtual T delta() { return delta_; };

    virtual T gamma() { return gamma_; };

    virtual T theta() { return theta_; };

    virtual T vanna() { return vanna_; };

    virtual T volga() { return volga_; };

    virtual T premium() { return premium_; };

    virtual void calculate() = 0;

    virtual void changeOption(Option<T> &option) {
        option_ = &option;
    }

    virtual void changeProcess(Process<T> &process) {
        process_ = &process;
    }


};

#endif  // ENGINE_HPP
