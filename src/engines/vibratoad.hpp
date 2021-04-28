#ifndef VIBRATOAD_VANILLA_HPP
#define VIBRATOAD_VANILLA_HPP

#include <autodiff/forward.hpp>
using namespace autodiff;


template<typename D>
class VibratoAD : PricingEngine<D> {

public:
    int n;
    int M;
    int Mz;
    double h;
    bool antithetic = true;

    VibratoAD(Option<D> *option, BlackScholesProcess<D> *process, int n, int M, int Mz) :
            PricingEngine<D>(option, process), n(n), M(M), Mz(Mz) {
        T = option->maturity();
        h = T / n;
    }

    D gamma() {
        D total = 0;
        for (int i = 0; i < M; ++i) {
            this->process_->resetState();
            deltaTangentProcess.resetState();
            for (int i = 0; i < n - 1; ++i) {
                D Z = normal();
                deltaTangentProcess.movePriceEuler(h, Z);
                this->process_->movePriceEuler(h, Z);
            }
            dual X = this->process_->priceState().value;
            total += derivative([&](auto x) { return this->_f_delta_wrt_price(x); }, wrt(X), at(X));
        }
        return exp(-this->process_->rate() * 2 * T) * total / M;
    }

    D vanna() override {
        D total = 0;
        for (int i = 0; i < M; ++i) {
            this->process_->resetState();
            vegaTangentProcess.resetState();
            for (int i = 0; i < n - 1; ++i) {
                D Z = normal();
                vegaTangentProcess.movePriceEuler(h, Z);
                this->process_->movePriceEuler(h, Z);
            }
            dual X = this->process_->priceState().value;
            total += derivative([&](auto x) { return this->_f_vega_wrt_x(x); }, wrt(X), at(X));
        }
        return exp(-this->process_->rate() * T) * total / M;
    }

    D volga() override {
        D total = 0;
        for (int i = 0; i < M; ++i) {
            this->process_->resetState();
            vegaTangentProcess.resetState();
            for (int i = 0; i < n - 1; ++i) {
                D Z = normal();
                vegaTangentProcess.movePriceEuler(h, Z);
                this->process_->movePriceEuler(h, Z);
            }
            dual X = this->process_->vol();
            total += derivative([&](auto x) { return this->_f_vega_wrt_sigma(x); }, wrt(X), at(X));
        }

        return exp(-this->process_->rate() * T) * total / M;
    }

    virtual ~VibratoAD() = default;


private:
    DeltaTangent<D> deltaTangentProcess = DeltaTangent<D>({this->process_->initialState.time, 1}, this->process_);
    VegaTangent<D> vegaTangentProcess = VegaTangent<D>({this->process_->initialState.time, 0}, this->process_);
    NormalDistribution<D> normal = NormalDistribution<D>(0, 1);
    D T;

    dual _f_delta_wrt_price(dual X) {
        D Z;
        dual espmu = 0;
        dual espsigma = 0;
        dual mun = X * (1 + this->process_->rate() * h);
        dual sigman = X * this->process_->vol() * sqrt(h);
        D dmun = deltaTangentProcess.dmun(h);
        D dsigman = deltaTangentProcess.dsigman(h);
        if (antithetic) {
            dual payoffPlus;
            dual payoffMinus;
            dual payoffMu;
            for (int j = 0; j < Mz; j++) {
                Z = normal();
                payoffPlus = this->option_->payoff(mun + sigman * Z);
                payoffMinus = this->option_->payoff(mun - sigman * Z);
                payoffMu = this->option_->payoff(mun);
                espmu += Z * (payoffPlus - payoffMinus) / (2 * sigman);
                espsigma += (Z * Z - 1) * (payoffPlus - 2 * payoffMu + payoffMinus) / (2 * sigman);
            }
        } else {
            dual p;
            for (int j = 0; j < Mz; j++) {
                Z = normal();
                p = this->option_->payoff(mun + sigman * Z);
                espmu += Z * p / (sigman);
                espsigma += (Z * Z - 1) * p / (2 * sigman * sigman);
            }
        }
        return (dmun * espmu + dsigman * espsigma) / Mz;
    }

    dual _f_vega_wrt_x(dual X) {
        D Z;
        dual espmu = 0;
        dual espsigma = 0;
        dual mun = X * (1 + this->process_->rate() * h);
        dual sigman = X * this->process_->vol() * sqrt(h);
        D dmun = vegaTangentProcess.dmun(h);
        dual dsigman = X + this->process_->vol() * vegaTangentProcess.priceState().value * sqrt(h);
        if (antithetic) {
            dual payoffPlus;
            dual payoffMinus;
            dual payoffMu;
            for (int j = 0; j < Mz; j++) {
                Z = normal();
                payoffPlus = this->option_->payoff(mun + sigman * Z);
                payoffMinus = this->option_->payoff(mun - sigman * Z);
                payoffMu = this->option_->payoff(mun);
                espmu += Z * (payoffPlus - payoffMinus) / (2 * sigman);
                espsigma += (Z * Z - 1) * (payoffPlus - 2 * payoffMu + payoffMinus) / (2 * sigman);
            }
        } else {
            dual p;
            for (int j = 0; j < Mz; j++) {
                Z = normal();
                p = this->option_->payoff(mun + sigman * Z);
                espmu += Z * p / (sigman);
                espsigma += (Z * Z - 1) * p / (2 * sigman * sigman);
            }
        }
        return (dmun * espmu + dsigman * espsigma) / Mz;
    }

    dual _f_vega_wrt_sigma(dual sigma) {
        D Z;
        dual espmu = 0;
        dual espsigma = 0;
        D mun = vegaTangentProcess.mun(h);
        dual sigman = this->process_->priceState().value * sigma * sqrt(h);
        D dmun = vegaTangentProcess.dmun(h);
        dual dsigman =
                (vegaTangentProcess.priceState().value + sigma * vegaTangentProcess.priceState().value) * sqrt(h);
        if (antithetic) {
            dual payoffPlus;
            dual payoffMinus;
            dual payoffMu;
            for (int j = 0; j < Mz; j++) {
                Z = normal();
                payoffPlus = this->option_->payoff(mun + sigman * Z);
                payoffMinus = this->option_->payoff(mun - sigman * Z);
                payoffMu = this->option_->payoff(mun);
                espmu += Z * (payoffPlus - payoffMinus) / (2 * sigman);
                espsigma += (Z * Z - 1) * (payoffPlus - 2 * payoffMu + payoffMinus) / (2 * sigman);
            }
        } else {
            dual p;
            for (int j = 0; j < Mz; j++) {
                Z = normal();
                p = this->option_->payoff(mun + sigman * Z);
                espmu += Z * p / (sigman);
                espsigma += 0.5 * (Z * Z - 1) * p / (sigman * sigman);
            }
        }
        return (dmun * espmu + dsigman * espsigma) / Mz;
    }
};

#endif  // VIBRATOAD_VANILLA_HPP
