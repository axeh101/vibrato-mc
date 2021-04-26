#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>
#include "../instruments/option.hpp"
#include "../processes/process.hpp"

template<typename T>
class PricingEngine {

protected:
    Option<T> *option_;
    Process<T> *process_;


public:
    PricingEngine(Option<T> *option, Process<T> *process)
            : option_(option), process_(process) {
    };

    virtual~PricingEngine() {

        option_ = nullptr;
        process_ = nullptr;
        delete option_;
        delete process_;

    };


    virtual T rho() { return nan(""); };

    virtual T vega() { return nan(""); };

    virtual T delta() { return nan(""); };

    virtual T gamma() { return nan(""); };

    virtual T theta() { return nan(""); };

    virtual T vanna() { return nan(""); };

    virtual T volga() { return nan(""); };

    virtual T premium() { return nan(""); };

    virtual void changeOption(Option<T> &option) {
        option_ = &option;
    }

    virtual void changeProcess(Process<T> &process) {
        process_ = &process;
    }

    friend std::ostream &operator<<(std::ostream &o, PricingEngine<T> &engine) {
        return engine.displayResults(o);
    }

    virtual std::ostream &displayResults(std::ostream &o) {
        return o << *option_ << std::endl << *process_ << std::endl <<
                 "Premium: " << premium() << std::endl <<
                 "Delta: " << delta() << std::endl <<
                 "Gamma: " << gamma() << std::endl <<
                 "Vega: " << vega() << std::endl <<
                 "Vanna: " << vanna() << std::endl <<
                 "Volga: " << volga() << std::endl <<
                 "Rho: " << rho() << std::endl <<
                 "Theta: " << theta() << std::endl;
    };

};

#endif  // ENGINE_HPP
