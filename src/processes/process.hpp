#ifndef PROCESS_HPP
#define PROCESS_HPP

#include "helper.hpp"
#include "../random/normal.hpp"

template <typename T>
class Process {
    protected:
        State<T> initialState_;
        State<T> currentState_;
        NormalDistribution<T> N_;
        std::string name_;
    public:
        virtual~Process() = default;
        Process(State<T>& initialState, std::string name)
            :initialState_(initialState), currentState_(initialState),
            N_(NormalDistribution<T>(0,1)), name_(name){};

        virtual T drift() const = 0;
        virtual T diffusion() const = 0;
        virtual State<T> nextState(double t) = 0;
        virtual State<T> currentState() {return currentState_;};
        State<T> initialState() {return initialState_;}

        friend std::ostream & operator<<(std::ostream & o, const Process<T>& opt)  {
            return opt.describe(o);
        }

        virtual std::ostream & describe(std::ostream & o) const
        {
            return  o << "Process name: " << name_ << std::endl <<
                "Initial State: " << initialState_ << std::endl <<
                "Current State: "<< currentState_ << std::endl;

        }

        virtual T eulerPriceDiff(double h) {
           return drift() * h + diffusion() * sqrt(h) * N_();
        }

        virtual Path<T> eulerDiscretization(int nsamples, double horizon)
        {
            Path<T> path = Path<T>(nsamples+1);
            path[0] = this->initialState_;
            double h = horizon / nsamples;
            for (int i = 1; i <= nsamples; ++i) {
                path[i] = {
                    i*h,
                    path[i-1].value + eulerPriceDiff(h)
                };
            }
            return path;

        }

        virtual State<T> moveToState(double t) {
            currentState_ = nextState(t);
            return currentState_;
        }


        virtual Path<T> generatePath(int nsamples, double horizon) {
            Path<T> path = Path<T>(nsamples+1);
            path[0] = this->initialState_;
            double h = horizon / nsamples;

            for (int i = 1; i <= nsamples; i++) {
                path[i] = this->moveToState(i*h);
            }
            return path;
        }

        virtual void resetState() {
            currentState_ = initialState_;
        }
};

#endif  // PROCESS_HPP
