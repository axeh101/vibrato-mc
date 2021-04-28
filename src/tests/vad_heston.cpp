#include <iostream>
#include "../all.hpp"
#include "helper.hpp"
using namespace std;

int main() {

    std::cout << "***** Staring vibrato AD for options (Heston) !" << std::endl;

    double maturity = 1;
    double strike = 100;
    double price = 50;
    double rate = .05;
    double vol = .32;
    int n = 100;
    int M = 10000;
    int Mz = 20;


    // Heston params
    double initialVol = 0.028087;
    double correlation = .5;
    double kappa = 2.931465; // mean reversion
    double eta = 0.101; // long run mean
    double volvol = 0.05;

    // Product definition
    VanillaOption<double> o(maturity, strike, OptionType::Call);

    // Heston & CIR process definition
    State<double> initialState = {0.0, price};
    CoxIngersollRossProcess<double> cir({0, initialVol}, kappa, eta, volvol);
    HestonProcess<double> heston(initialState, &cir, rate, correlation);
    BlackScholesProcess<double> bs(initialState, rate, vol);

    // Pricing engines definition
    auto ve = VibratoAD<double>(&o, &bs, n, M, Mz);
    auto be = AnalyticalDigitalBS<double>(&o, &bs);

    size_t vecSize = 100;
    int step = 1;

    // Black Scholes
    Helper::generateGreek(&o, &bs, [&](void) -> double { return be.gamma(); }, price, vecSize, step,
                          "_analytic_h_gamma");
    Helper::generateGreek(&o, &bs, [&](void) -> double { return be.vanna(); }, price, vecSize, step,
                          "_analytic_h_vanna");
    Helper::generateGreek(&o, &bs, [&](void) -> double { return be.volga(); }, price, vecSize, step,
                          "_analytic_h_volga");


    ve.antithetic = false;
    Helper::generateGreek(&o, &heston, [&](void) -> double { return ve.gamma(); }, price, vecSize, step,
                          "_vibrato_heston_ad_gamma");
    Helper::generateGreek(&o, &heston, [&](void) -> double { return ve.vanna(); }, price, vecSize, step,
                          "_vibrato_heston_ad_vanna");
    Helper::generateGreek(&o, &heston, [&](void) -> double { return ve.volga(); }, price, vecSize, step,
                          "_vibrato_heston_ad_volga");

    // Antithetic outputs
    ve.antithetic = true;
    Helper::generateGreek(&o, &heston, [&](void) -> double { return ve.gamma(); }, price, vecSize, step,
                          "_vibrato_heston_ad_gamma_anti");
    Helper::generateGreek(&o, &heston, [&](void) -> double { return ve.vanna(); }, price, vecSize, step,
                          "_vibrato_heston_ad_vanna_anti");
    Helper::generateGreek(&o, &heston, [&](void) -> double { return ve.volga(); }, price, vecSize, step,
                          "_vibrato_heston_ad_volga_anti");


    std::cout << "***** Vibrato  AD for vanilla (Heston) terminated!" << std::endl;

    return 0;
}
