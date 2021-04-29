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
    D h;
    bool antithetic = true;

    VibratoAD(Option<D> *option, Process<D> *process, int n, int M, int Mz) :
            PricingEngine<D>(option, process), n(n), M(M), Mz(Mz) {
        T = option->maturity();
        h = T / n;
    }

    D gamma() {
        D total = 0;
        // Fonction permettant de calculer le delta dont on va calculer la dérivée par AD.
        // On utilise une expression lambda car la librairie ne supporte pas les méthodes
        // de classe
        auto f = [&](auto x) { return this->_f_delta(x); };

        for (int i = 0; i < M; ++i) {
            this->process_->resetState();
            deltaTangentProcess.resetState();
            for (int i = 0; i < n - 1; ++i) {
                D Z = normal();
                deltaTangentProcess.movePriceEuler(h, Z);
                this->process_->movePriceEuler(h, Z);
            }
            D X = this->process_->priceState().value;
            total += derivative(f, wrt(X), at(X));
        }
        return exp(-this->process_->rate() * T) * total / (M * Mz);
    }

    D vanna() override {
        D total = 0;
        // Fonction permettant de calculer le vega dont on va calculer la dérivée pa rapport a X par AD.
        auto f = [&](auto x, auto y) { return this->_f_vega(x, y); };
        for (int i = 0; i < M; ++i) {
            this->process_->resetState();
            vegaTangentProcess.resetState();
            for (int i = 0; i < n - 1; ++i) {
                D Z = normal();
                vegaTangentProcess.movePriceEuler(h, Z);
                this->process_->movePriceEuler(h, Z);
            }
            D sigma = this->process_->vol();
            D X = this->process_->priceState().value;
            total += derivative(f, wrt(X), at(X, sigma));
        }
        return exp(-this->process_->rate() * T) * total / (M * Mz);
    }

    D volga() override {
        D total = 0;
        // Fonction permettant de calculer le vega dont on va calculer la dérivée pa rapport a sigma par AD.
        auto f = [&](auto x, auto y) { return this->_f_vega(x, y); };
        for (int i = 0; i < M; ++i) {
            this->process_->resetState();
            vegaTangentProcess.resetState();
            for (int i = 0; i < n - 1; ++i) {
                D Z = normal();
                vegaTangentProcess.movePriceEuler(h, Z);
                this->process_->movePriceEuler(h, Z);
            }
            D sigma = this->process_->vol();
            D X = this->process_->priceState().value;
            total += derivative(f, wrt(sigma), at(X, sigma));
        }

        return exp(-this->process_->rate() * T) * total / (M * Mz);
    }

    virtual ~VibratoAD() = default;


private:
    DeltaTangent<D> deltaTangentProcess = DeltaTangent<D>({this->process_->initialState.time, 1}, this->process_);
    VegaTangent<D> vegaTangentProcess = VegaTangent<D>({this->process_->initialState.time, 0}, this->process_);
    NormalDistribution<D> normal = NormalDistribution<D>(0, 1);
    D T;

    D _f_delta(D X) {
        D Z;
        D espmu = 0;
        D espsigma = 0;
        D mun = X * (1 + this->process_->rate() * h);
        D sigman = X * this->process_->vol() * sqrt(h);
        D dmun = deltaTangentProcess.dmun(h);
        D dsigman = deltaTangentProcess.dsigman(h);
        if (antithetic) {
            D payoffPlus;
            D payoffMinus;
            D payoffMu;
            for (int j = 0; j < Mz; j++) {
                Z = normal();
                payoffPlus = this->option_->payoff(mun + sigman * Z);
                payoffMinus = this->option_->payoff(mun - sigman * Z);
                payoffMu = this->option_->payoff(mun);
                espmu += Z * (payoffPlus - payoffMinus) / (2 * sigman);
                espsigma += (Z * Z - 1) * (payoffPlus - 2 * payoffMu + payoffMinus) / (2 * sigman);
            }
        } else {
            D p;
            for (int j = 0; j < Mz; j++) {
                Z = normal();
                p = this->option_->payoff(mun + sigman * Z);
                espmu += Z * p / (sigman);
                espsigma += (Z * Z - 1) * p / (2 * sigman * sigman);
            }
        }
        return (dmun * espmu + dsigman * espsigma);
    }

    D _f_vega(D X, D sigma) {
        D Z;
        D espmu = 0;
        D espsigma = 0;
        D sigman = X * sigma * sqrt(h);
        D dmun = vegaTangentProcess.dmun(h);
        D mun = X * (1 + this->process_->rate() * h);
        D dsigman = X + sigma * vegaTangentProcess.priceState().value * sqrt(h);
        if (antithetic) {
            D payoffPlus;
            D payoffMinus;
            D payoffMu;
            for (int j = 0; j < Mz; j++) {
                Z = normal();
                payoffPlus = this->option_->payoff(mun + sigman * Z);
                payoffMinus = this->option_->payoff(mun - sigman * Z);
                payoffMu = this->option_->payoff(mun);
                espmu += Z * (payoffPlus - payoffMinus) / (2 * sigman);
                espsigma += (Z * Z - 1) * (payoffPlus - 2 * payoffMu + payoffMinus) / (2 * sigman);
            }
        } else {
            D p;
            for (int j = 0; j < Mz; j++) {
                Z = normal();
                p = this->option_->payoff(mun + sigman * Z);
                espmu += Z * p / (sigman);
                espsigma += (Z * Z - 1) * p / (2 * sigman * sigman);
            }
        }
        return (dmun * espmu + dsigman * espsigma);
    }
};

#endif  // VIBRATOAD_VANILLA_HPP
