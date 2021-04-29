#ifndef VIBRATO_HPP
#define VIBRATO_HPP

#include <cmath>
#include <list>
#include "engine.hpp"
#include "../random/normal.hpp"
#include "../processes/blackscholes.hpp"
#include "../processes/helper.hpp"
#include "../processes/tangent/all.hpp"


template<typename D>
class Vibrato : public PricingEngine<D> {

public:
    int n;
    int M;
    int Mz;
    double h;
    bool antithetic = true;

    Vibrato(Option<D> *option, Process<D> *process, int n, int M, int Mz) :
            PricingEngine<D>(option, process), n(n), M(M), Mz(Mz) {
        T = option->maturity();
        h = T / n;
    }

    virtual ~Vibrato() = default;

    virtual D premium() override {
        D r = this->process_->rate();
        D total = 0.;

        for (int i = 0; i < M; ++i) {
            this->process_->resetState();
            for (int i = 0; i < n - 1; ++i) {
                this->process_->movePriceEuler(h, normal());
            }
            // Replication step
            D value = this->process_->priceState().value;
            D sigma = this->process_->vol();
            D subtotal = 0.;
            D mun = value * (1 + r * h);
            D sigman = value * sigma * sqrt(h);
            for (int j = 1; j <= Mz; j++) {
                subtotal += this->option_->payoff(mun + sigman * normal());
            }
            // End of Replication step
            total += subtotal / Mz;
        }
        return exp(-r * T) * total / M;
    }

    virtual D delta() override {
        D total = 0.;
        for (int i = 0; i < M; ++i) {
            this->process_->resetState();
            deltaTangentProcess.resetState();
            for (int i = 0; i < n - 1; ++i) {
                D Z = normal();
                deltaTangentProcess.movePriceEuler(h, Z);
                this->process_->movePriceEuler(h, Z);

            }
            total += _firstOrderVibrato(this->deltaTangentProcess.mun(h),
                                        this->deltaTangentProcess.dmun(h),
                                        this->deltaTangentProcess.sigman(h),
                                        this->deltaTangentProcess.dsigman(h));
        }
        return exp(-this->process_->rate() * T) * total / M;
    }

    virtual D vega() override {
        D total = 0.;
        for (int i = 0; i < M; ++i) {
            this->process_->resetState();
            vegaTangentProcess.resetState();
            for (int i = 0; i < n - 1; ++i) {
                D Z = normal();
                vegaTangentProcess.movePriceEuler(h, Z);
                this->process_->movePriceEuler(h, Z);
            }
            total += _firstOrderVibrato(this->vegaTangentProcess.mun(h),
                                        this->vegaTangentProcess.dmun(h),
                                        this->vegaTangentProcess.sigman(h),
                                        this->vegaTangentProcess.dsigman(h));
        }
        return exp(-this->process_->rate() * T) * total / M;
    }

    virtual D rho() override {
        D total = 0.;
        for (int i = 0; i < M; ++i) {
            this->process_->resetState();
            rhoTangentProcess.resetState();
            for (int i = 0; i < n - 1; ++i) {
                D Z = normal();
                rhoTangentProcess.movePriceEuler(h, Z);
                this->process_->movePriceEuler(h, Z);
            }
            total += _firstOrderVibrato(this->rhoTangentProcess.mun(h),
                                        this->rhoTangentProcess.dmun(h),
                                        this->rhoTangentProcess.sigman(h),
                                        this->rhoTangentProcess.dsigman(h));
        }
        return exp(-this->process_->rate() * T) * total / M - T * premium();
    }

    virtual D theta() override {
        D total = 0.;
        for (int i = 0; i < M; ++i) {
            this->process_->resetState();
            thetaTangentProcess.resetState();
            for (int i = 0; i < n - 1; ++i) {
                D Z = normal();
                thetaTangentProcess.movePriceEuler(h, Z);
                this->process_->movePriceEuler(h, Z);
            }
            total += _firstOrderVibrato(this->thetaTangentProcess.mun(h),
                                        this->thetaTangentProcess.dmun(h),
                                        this->thetaTangentProcess.sigman(h),
                                        this->thetaTangentProcess.dsigman(h));
        }
        return exp(-this->process_->rate() * T) * (total / M - this->process_->rate() * premium());
    }

    virtual D gamma() override {
        D total = 0.;
        for (int i = 0; i < M; ++i) {
            this->process_->resetState();
            deltaTangentProcess.resetState();
            for (int i = 0; i < n - 1; ++i) {
                D Z = normal();
                deltaTangentProcess.movePriceEuler(h, Z);
                this->process_->movePriceEuler(h, Z);
            }
            total += _secondOrderVibrato(this->deltaTangentProcess.mun(h),
                                         this->deltaTangentProcess.sigman(h),
                                         this->deltaTangentProcess.dmun(h),
                                         this->deltaTangentProcess.dsigman(h),
                                         this->deltaTangentProcess.dmun(h),
                                         this->deltaTangentProcess.dsigman(h));
        }
        return exp(-this->process_->rate() * T) * total / M;
    }

    virtual D vanna() override {
        D total = 0.;
        for (int i = 0; i < M; ++i) {
            this->process_->resetState();
            vegaTangentProcess.resetState();
            deltaTangentProcess.resetState();
            vannaTangentProcess.resetState();
            for (int i = 0; i < n - 1; ++i) {
                D Z = normal();
                vannaTangentProcess.movePriceEuler(h, Z);
                deltaTangentProcess.movePriceEuler(h, Z);
                vegaTangentProcess.movePriceEuler(h, Z);
                this->process_->movePriceEuler(h, Z);
            }
            total += _secondOrderVibrato(this->vegaTangentProcess.mun(h),
                                         this->vegaTangentProcess.sigman(h),
                                         this->vegaTangentProcess.dmun(h),
                                         this->vegaTangentProcess.dsigman(h),
                                         this->deltaTangentProcess.dmun(h),
                                         this->deltaTangentProcess.dsigman(h),
                                         this->vannaTangentProcess.dmun(h),
                                         this->vannaTangentProcess.dsigman(h));
        }
        return exp(-this->process_->rate() * T) * total / M;
    }

    virtual D volga() override {
        D total = 0.;
        for (int i = 0; i < M; ++i) {
            this->process_->resetState();
            vegaTangentProcess.resetState();
            for (int i = 0; i < n - 1; ++i) {
                D Z = normal();
                vegaTangentProcess.movePriceEuler(h, Z);
                this->process_->movePriceEuler(h, Z);
            }
            total += _secondOrderVibrato(this->vegaTangentProcess.mun(h),
                                         this->vegaTangentProcess.sigman(h),
                                         this->vegaTangentProcess.dmun(h),
                                         this->vegaTangentProcess.dsigman(h),
                                         this->vegaTangentProcess.dmun(h),
                                         this->vegaTangentProcess.dsigman(h));
        }
        return exp(-this->process_->rate() * T) * total / M;
    }

//    virtual D volga() override{
//        D total = 0.;
//        for (int i = 0; i < M; ++i) {
//            this->process_->resetState();
//            vegaTangentProcess.resetState();
//            for (int i = 0; i < n - 1; ++i) {
//                D Z = normal();
//                volgaTangentProcess.movePriceEuler(h, Z);
//                vegaTangentProcess.movePriceEuler(h, Z);
//                this->process_->movePriceEuler(h, Z);
//            }
//            total += _secondOrderVibrato(this->vegaTangentProcess.mun(h),
//                                         this->vegaTangentProcess.sigman(h),
//                                         this->vegaTangentProcess.dmun(h),
//                                         this->vegaTangentProcess.dsigman(h),
//                                         this->vegaTangentProcess.dmun(h),
//                                         this->vegaTangentProcess.dsigman(h),
//                                         this->volgaTangentProcess.dmun(h),
//                                         this->volgaTangentProcess.dsigman(h));
//        }
//        return exp(-this->process_->rate() * T) * total / M;
//    }

private:
    DeltaTangent<D> deltaTangentProcess = DeltaTangent<D>({this->process_->initialState.time, 1}, this->process_);
    VegaTangent<D> vegaTangentProcess = VegaTangent<D>({this->process_->initialState.time, 0}, this->process_);
    RhoTangent<D> rhoTangentProcess = RhoTangent<D>({this->process_->initialState.time, 0}, this->process_);
    ThetaTangent<D> thetaTangentProcess = ThetaTangent<D>({this->process_->initialState.time, 0}, this->process_);
    VannaTangent<D> vannaTangentProcess = VannaTangent<D>({this->process_->initialState.time, 0},
                                                          &deltaTangentProcess, &vegaTangentProcess);
    VolgaTangent<D> volgaTangentProcess = VolgaTangent<D>({this->process_->initialState.time, 0}, &vegaTangentProcess);

    NormalDistribution<D> normal = NormalDistribution<D>(0, 1);
    D T;


    D _firstOrderVibrato(D mun, D dmun, D sigman, D dsigman) {
        D Z;
        D espmu = 0;
        D espsigma = 0;
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
            D payoff;
            for (int j = 0; j < Mz; j++) {
                Z = normal();
                payoff = this->option_->payoff(mun + sigman * Z);
                espmu += Z * payoff / (sigman);
                espsigma += 0.5 * (Z * Z - 1) * payoff / (sigman * sigman);
            }
        }
        return (dmun * espmu + dsigman * espsigma) / Mz;

    }

    D _secondOrderVibrato(D mun, D sigman, D mun1, D sigman1, D mun2, D sigman2, D mun12 = 0, D sigman12 = 0) {

        D espMuMu = 0;
        D espMu2 = 0;

        D espSigmaSigma = 0;
        D espSigma2 = 0;
        D espMuSigma = 0;

        D Z;
        D Z2;
        D Z4;

        if (antithetic) {
            D payoffPlus;
            D payoffMinus;
            D payoffMu;
            for (int j = 0; j < Mz; j++) {
                Z = normal();
                Z2 = Z * Z;
                Z4 = Z2 * Z2;
                payoffPlus = this->option_->payoff(mun + sigman * Z);
                payoffMinus = this->option_->payoff(mun - sigman * Z);
                payoffMu = this->option_->payoff(mun);
                espMu2 += Z * (payoffPlus - payoffMinus) / (2 * sigman);
                espMuMu += (Z2 - 1) * (payoffPlus - 2 * payoffMu + payoffMinus) / (2 * sigman * sigman);
                espSigmaSigma += (Z4 - 5 * Z2 + 2) * (payoffPlus - 2 * payoffMu + payoffMinus) / (2 * sigman * sigman);
                espSigma2 += (Z2 - 1) * (payoffPlus - 2 * payoffMu + payoffMinus) / (2 * sigman);
                espMuSigma += Z * (Z2 - 3) * (payoffPlus - payoffMinus) / (sigman * sigman);
            }
        } else {
            D payoff;
            for (int j = 0; j < Mz; j++) {
                Z = normal();
                Z2 = Z * Z;
                Z4 = Z2 * Z2;
                payoff = this->option_->payoff(mun + sigman * Z);
                espMuMu += (Z2 - 1) * payoff / (sigman * sigman);
                espSigmaSigma += (Z4 - 5 * Z2 + 2) * payoff / (sigman * sigman);
                espMuSigma += Z * (Z2 - 3) * payoff / (sigman * sigman);
                espMu2 += Z * payoff / sigman;
                espSigma2 += (Z2 - 1) * payoff / sigman;
            }
        }
        return (mun1 * mun2 * espMuMu + sigman1 * sigman2 * espSigmaSigma + mun12 * espMu2 + sigman12 * espSigma2 +
                (sigman1 * mun2 + sigman2 * mun1) * espMuSigma) / Mz;

    }
};

#endif  // VIBRATO_HPP
