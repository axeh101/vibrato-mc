#ifndef PROCESS_HPP
#define PROCESS_HPP

#include "helper.hpp"
#include "../random/normal.hpp"

template<typename T>
class Process {

public:
    State<T> initialState;

    virtual ~Process() = default;

    Process(State<T> &initialState, std::string name) :
            initialState(initialState), priceState_(initialState), name_(name) {
    }

    virtual T drift() const = 0;

    virtual T diffusion() const = 0;

    virtual Path<T> eulerDiscretization(int nsamples, double horizon) {
        NormalDistribution<T> N = NormalDistribution<T>(0, 1);
        resetState();
        Path<T> path = Path<T>(nsamples + 1);
        path[0] = initialState;
        double h = horizon / nsamples;
        for (int i = 1; i <= nsamples; ++i) {
            path[i] = movePriceEuler(h, N());
        }
        return path;

    }



    virtual State<T> movePriceEuler(double h, T Z) {
        this->priceState_ = this->nextPriceEuler(h, Z);
        return this->priceState_;
    }

    virtual State<T> nextPriceEuler(double h, T Z) {
        return {
                priceState_.time + h,
                priceState_.value + eulerPriceDiff(h, Z),
        };
    }

    virtual T eulerPriceDiff(double h, T Z) {
        return this->drift() * h + this->diffusion() * sqrt(h) * Z;
    }

    virtual void resetState() {
        priceState_ = initialState;
    }

    virtual State<T> priceState() {
        return priceState_;
    }


    friend std::ostream &operator<<(std::ostream &o, const Process<T> &opt) {
        return opt.describe(o);
    }

    virtual std::ostream &describe(std::ostream &o) const {
        return o << "Process name: " << name_ << std::endl << "Initial State: "
                 << initialState << std::endl << "Price State: " << priceState_ << std::endl;

    }

    virtual T vol() const = 0;

    virtual T rate() const = 0;


    /**
     * computes db(theta, Xkn)/dsigma
     */
    virtual T diffDriftVega() const { return 0; }


    /**
     * computes db(theta, Xkn)/dr
     */
    virtual T diffDriftRho() const { return 0; }

    /**
     * computes dsigma(theta, Xkn)/dsigma
     */
    virtual T diffDiffusionVega() const { return 0; }

    /**
     * computes dsigma(theta, Xkn)/dr
     */
    virtual T diffDiffusionRho() const { return 0; }

    /**
     * computes db(theta, Xkn)/dx
     */
    virtual T diffDriftX() const { return 0; }

    /**
     * computes dsigma(theta, Xkn)/dx
     */
    virtual T diffDiffusionX() const { return 0; }

    /**
     * computes d²b(theta, Xkn)/dx²
     */
    virtual T diffDriftX2() const { return 0; }

    /**
     * computes d²sigma(theta, Xkn)/dx²
     */
    virtual T diffDiffusionX2() const { return 0; }

    /**
     * computes d²sigma(theta, Xkn)/dx dsigma
     */
    virtual T diffDiffusionSigmaX() const { return 0; }

    /**
   * computes d²sigma(theta, Xkn)/dsigma dsigma
   */
    virtual T diffDiffusionSigma2() const { return 0; }

protected:
    T rate_;
    T vol_;
    State<T> priceState_;
    std::string name_;
};

template<typename D>
class TangentProcess : public Process<D> {
public:
    Process<D> *parent_;

    virtual ~TangentProcess() {
        parent_ = nullptr;
        delete parent_;
    }

    TangentProcess(State<D> initialState, std::string name, Process<D> *parent) :
            Process<D>(initialState, name), parent_(parent) {}

    virtual D mun(double h) {
        return this->parent_->priceState().value + this->parent_->drift() * h;
    }

    virtual D dmun(double h) {
        return this->priceState_.value + this->drift() * h;
    }

    virtual D sigman(double h) {
        return this->parent_->diffusion() * sqrt(h);
    }

    virtual D dsigman(double h) {
        return this->diffusion() * sqrt(h);
    }

    virtual D vol() const override {
        return this->parent_->vol();
    }

    virtual D rate() const override {
        return this->parent_->rate();
    }

};

#endif  // PROCESS_HPP
