#ifndef BLACKSCHOLES_HPP
#define BLACKSCHOLES_HPP

#include "process.hpp"
#include "../random/normal.hpp"

template <typename T>
class BlackScholesProcess : public Process<T>
{

    public:
        BlackScholesProcess(State<T> initialState, T rate , T vol):
            Process<T>(initialState, "Black Scholes"),rate_(rate), vol_(vol){};

        virtual T drift() const {return rate_ * this->currentState_.value;};
        virtual T diffusion() const {return vol_ * this->currentState_.value ;};

        virtual State<T> nextState(double t) {
            assert(t > this->currentState_.time);
            double dt = (t-this->currentState_.time);
            T  currentValue =  this->currentState_.value;
            T dwt = sqrt(dt) * this->N_();
            T newValue = currentValue * exp( (rate_ - 0.5 * vol_ * vol_) * dt + vol_* dwt);
            return {t, newValue};
        }

        virtual T vol() const {return vol_;}
        virtual T rate() const {return rate_;}

        virtual std::ostream & describe(std::ostream & o) const
        {
            return  Process<T>::describe(o)  <<
                "Rate: " << rate_ << std::endl <<
                "Volatility: "<< vol_ << std::endl;
        };

    private:
        T rate_;
        T vol_;
};

#endif  // BLACKSCHOLES_HPP
