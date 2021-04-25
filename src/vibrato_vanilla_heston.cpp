#include <iostream>
#include "all.hpp"

using namespace std;

int main() {

    std::cout << "***** Staring vibrato for options (Heston) !" << std::endl;

    const std::string destination = "src/python/datasets/";

    double maturity = 1;
    double strike = 90;
    double rate = .00135;
    int n = 100;
    int M = 10000;
    int Mz = 2;
    double vol = .2;
    // Heston params
    double initialVol = 0.028087;
    double correlation = .5;
    double kappa = 2.931465; // mean reversion
    double eta = 0.101; // long run mean
    double volvol = 0.01;

    // Product definition
    VanillaOption<double> call(maturity, strike, OptionType::Call);

    // Heston & CIR process definition
    State<double> initialState = {0.0, 60};
    CoxIngersollRossProcess<double> cir({0, initialVol}, kappa, eta, volvol);
    HestonProcess<double> heston(initialState, &cir, rate, correlation);
    BlackScholesProcess<double> bs(initialState, rate, vol);

    // Pricing engines definition
    auto ve = Vibrato<double>(&call, &bs, n, M, Mz);
    auto be = AnalyticalDigitalBS<double>(&call, &bs);

    double vecSize = 200;
    // Premium tests
    auto *vibratoPremium = new Path<double>(vecSize);
    auto *analyticPremium = new Path<double>(vecSize);

    // delta tests
    auto *vibratoDelta = new Path<double>(vecSize);
    auto *analyticDelta = new Path<double>(vecSize);
    auto *vibratoDeltaAntithetic = new Path<double>(vecSize);

    // vega tests
    auto *vibratoVega = new Path<double>(vecSize);
    auto *analyticVega = new Path<double>(vecSize);
    auto *vibratoVegaAntithetic = new Path<double>(vecSize);

    // rho tests
    auto *vibratoRho = new Path<double>(vecSize);
    auto *analyticRho = new Path<double>(vecSize);
    auto *vibratoRhoAntithetic = new Path<double>(vecSize);

    double price = 60;
    for (int i = 0; i < vecSize; ++i) {
        bs.initialState.value = price;
        be.calculate();
        (*analyticPremium)[i] = {price, be.premium()};
        (*analyticDelta)[i] = {price, be.delta()};
        (*analyticVega)[i] = {price, be.vega()};
        (*analyticRho)[i] = {price, be.rho()};

        ve.antithetic = true;
        ve.calculate();
        (*vibratoPremium)[i] = {price, ve.premium()};
        (*vibratoVegaAntithetic)[i] = {price, ve.vega()};
        (*vibratoDeltaAntithetic)[i] = {price, ve.delta()};
        (*vibratoRhoAntithetic)[i] = {price, ve.rho()};

        ve.antithetic = false;
        ve.calculate();
        (*vibratoDelta)[i] = {price, ve.delta()};
        (*vibratoVega)[i] = {price, ve.vega()};
        (*vibratoRho)[i] = {price, ve.rho()};

        price += 0.5;
    }
    vect2csv(destination + "call_heston_analytic_premium", *analyticPremium);
    vect2csv(destination + "call_heston_analytic_delta", *analyticDelta);
    vect2csv(destination + "call_heston_analytic_vega", *analyticVega);
    vect2csv(destination + "call_heston_analytic_rho", *analyticRho);

    vect2csv(destination + "call_heston_vibrato_premium", *vibratoPremium);

    vect2csv(destination + "call_heston_vibrato_delta", *vibratoDelta);
    vect2csv(destination + "call_heston_vibrato_delta_antithetic", *vibratoDeltaAntithetic);
    vect2csv(destination + "call_heston_vibrato_vega", *vibratoVega);
    vect2csv(destination + "call_heston_vibrato_vega_antithetic", *vibratoVegaAntithetic);
    vect2csv(destination + "call_heston_vibrato_rho", *vibratoRho);
    vect2csv(destination + "call_heston_vibrato_rho_antithetic", *vibratoRhoAntithetic);

    delete vibratoPremium;
    delete analyticPremium;
    delete analyticDelta;
    delete analyticRho;

    delete vibratoDelta;
    delete vibratoVega;
    delete vibratoRho;

    delete vibratoDeltaAntithetic;
    delete vibratoVegaAntithetic;
    delete vibratoRhoAntithetic;

    std::cout << "***** Vibrato for vanilla (Heston) terminated!" << std::endl;

    return 0;
}
