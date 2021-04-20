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

        virtual State<T> nextIto(double h) {
            return {
                this->currentState_.time + h, 
                ito(this->currentState_.value, h)
            };
        }

        virtual T vol() const {return vol_;}
        virtual T rate() const {return rate_;}

        virtual std::ostream & describe(std::ostream & o) const
        {
            return  Process<T>::describe(o)  <<
                "Rate: " << rate_ << std::endl <<
                "Volatility: "<< vol_ << std::endl;
        };

        virtual State<T> moveIto(double t) {
            this->currentState_ = nextIto(t);
            return this->currentState_;
        }

        virtual Path<T> generatePath(int nsamples, double horizon) {
            Path<T> path = Path<T>(nsamples+1);
            path[0] = this->initialState_;
            double h = horizon / nsamples;
            for (int i = 1; i <= nsamples; i++) {
                path[i] = this->moveIto(h);
            }
            return path;
        }
    private:
        T rate_;
        T vol_;

        T ito(T value, double h) {
            T vol2= vol_ * vol_;
            T dWt = sqrt(h) * this->N_(); 
            T newValue = value * exp( (rate_ - 0.5 * vol2) * h + vol_* dWt);
            return newValue;
        }

};

#endif  // BLACKSCHOLES_HPP
