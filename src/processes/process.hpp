#ifndef PROCESS_HPP
#define PROCESS_HPP

#include "helper.hpp"
#include "../random/normal.hpp"

template <typename T>
class Process {
    protected:
        State<T> initialState_;
        State<T> priceState_;
        State<T> tangentState_;

        NormalDistribution<T> N_ = NormalDistribution<T>(0,1);
        std::string name_;

    public:
        virtual~Process() = default;
        Process(State<T>& initialState, std::string name)
            :initialState_(initialState), priceState_(initialState), name_(name)
        {
        	tangentState_ = {initialState.time, 1};
        };

        virtual T drift() const = 0;
        virtual T diffusion() const = 0;

        virtual T tangentDrift() const = 0;
        virtual T tangentDiffusion() const = 0;

        virtual T eulerPriceDiff(double h) {
           return drift() * h + diffusion() * sqrt(h) * N_();
        }

        virtual T eulerTangentDiff(double h) {
           return tangentDrift() * h + tangentDiffusion() * sqrt(h) * N_();
        }

        virtual Path<T> eulerDiscretization(int nsamples, double horizon)
        {
            resetState();
            Path<T> path = Path<T>(nsamples+1);
            path[0] = this->initialState_;
            double h = horizon / nsamples;
            for (int i = 1; i <= nsamples; ++i) {
                path[i] = movePriceEuler(h);
            }
            return path;

        }

        virtual State<T> nextPriceEuler(double h) {
            return {
                this->priceState_.time + h,
                this->priceState_.value + eulerPriceDiff(h),
            };
        }

        virtual State<T> movePriceEuler(double h) {
            priceState_ = nextPriceEuler(h);
            return priceState_;
        }

        virtual State<T> nextTangentEuler(double h) {
            return {
                this->tangentState_.time + h,
                this->tangentState_.value + eulerTangentDiff(h),
            };
        }

        virtual State<T> moveTangentEuler(double h) {
            priceState_ = nextTangentEuler(h);
            return priceState_;
        }

        virtual void resetState() { priceState_ = initialState_;}
        virtual void resetTangentState(){tangentState_ = {initialState_.time, 1};}
        virtual State<T> initialState() {return initialState_;}
        virtual State<T> priceState() {return priceState_;};
        virtual State<T> tangentState() {return tangentState_;};

        friend std::ostream & operator<<(std::ostream & o, const Process<T>& opt)  {
            return opt.describe(o);
        }

        virtual std::ostream & describe(std::ostream & o) const
        {
            return  o << "Process name: " << name_ << std::endl <<
                "Initial State: " << initialState_ << std::endl <<
                "Price State: "<< priceState_ << std::endl <<
                "Tangent State: "<< tangentState_ << std::endl;


        }
};

#endif  // PROCESS_HPP
