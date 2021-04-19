#ifndef PROCESS_HPP
#define PROCESS_HPP

#include "helper.hpp"
#include "../random/distribution.hpp"

template <typename T>
class Process {
    protected:
        State<T> initialState_;
        State<T> currentState_;
        std::string name_;
    public:
        Process(State<T>& initialState, std::string name):
            initialState_(initialState), currentState_(initialState), name_(name){};
        virtual~Process() = default;

        virtual Path<T> generatePath(int nsamples, double horizon) = 0;
        virtual T drift() const = 0;
        virtual T diffusion() const = 0;
        virtual State<T> nextState(double t) = 0;

        virtual State<T> moveToState(double t) {
            currentState_ = nextState(t);
            return currentState_;
        }
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

        };
};

#endif  // PROCESS_HPP
