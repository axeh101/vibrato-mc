#include <iostream>
#include "../all.hpp"
#include "helper.hpp"

using namespace std;

int main() {

    std::cout << "***** Staring vibrato for options (Heston) !" << std::endl;

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
    auto ve = Vibrato<double>(&o, &heston, n, M, Mz);
    auto be = AnalyticalVanillaBS<double>(&o, &bs);


    size_t vecSize = 100;
    int step = 1;

    // Black Scholes
    Helper<double>::generateGreek(&o, &bs, [&](void) -> double { return be.premium(); }, price, vecSize, step,
                                  "_analytic_h_premium");

    Helper<double>::generateGreek(&o, &bs, [&](void) -> double { return be.delta(); }, price, vecSize, step,
                                  "_analytic_h_delta");
    Helper<double>::generateGreek(&o, &bs, [&](void) -> double { return be.vega(); }, price, vecSize, step,
                                  "_analytic_h_vega");
    Helper<double>::generateGreek(&o, &bs, [&](void) -> double { return be.rho(); }, price, vecSize, step,
                                  "_analytic_h_rho");
    // Second order greeks
    Helper<double>::generateGreek(&o, &bs, [&](void) -> double { return be.gamma(); }, price, vecSize, step,
                                  "_analytic_h_gamma");
    Helper<double>::generateGreek(&o, &bs, [&](void) -> double { return be.vanna(); }, price, vecSize, step,
                                  "_analytic_h_vanna");
    Helper<double>::generateGreek(&o, &bs, [&](void) -> double { return be.volga(); }, price, vecSize, step,
                                  "_analytic_h_volga");

    // Vibrato
    Helper<double>::generateGreek(&o, &heston, [&](void) -> double { return ve.premium(); }, price, vecSize, step,
                                  "_vibrato_heston_premium");

    ve.antithetic = false;
    // First order greeks
    Helper<double>::generateGreek(&o, &heston, [&](void) -> double { return ve.delta(); }, price, vecSize, step,
                                  "_vibrato_heston_delta");
    Helper<double>::generateGreek(&o, &heston, [&](void) -> double { return ve.vega(); }, price, vecSize, step,
                                  "_vibrato_heston_vega");
    Helper<double>::generateGreek(&o, &heston, [&](void) -> double { return ve.rho(); }, price, vecSize, step,
                                  "_vibrato_heston_rho");
    // Second order greeks
    Helper<double>::generateGreek(&o, &heston, [&](void) -> double { return ve.gamma(); }, price, vecSize, step,
                                  "_vibrato_heston_gamma");
    Helper<double>::generateGreek(&o, &heston, [&](void) -> double { return ve.vanna(); }, price, vecSize, step,
                                  "_vibrato_heston_vanna");
    Helper<double>::generateGreek(&o, &heston, [&](void) -> double { return ve.volga(); }, price, vecSize, step,
                                  "_vibrato_heston_volga");

    // Antithetic outputs
    ve.antithetic = true;
    // First order greeks
    Helper<double>::generateGreek(&o, &heston, [&](void) -> double { return ve.delta(); }, price, vecSize, step,
                                  "_vibrato_heston_delta_anti");
    Helper<double>::generateGreek(&o, &heston, [&](void) -> double { return ve.vega(); }, price, vecSize, step,
                                  "_vibrato_heston_vega_anti");
    Helper<double>::generateGreek(&o, &heston, [&](void) -> double { return ve.rho(); }, price, vecSize, step,
                                  "_vibrato_heston_rho_anti");
    // Second order greeks
    Helper<double>::generateGreek(&o, &heston, [&](void) -> double { return ve.gamma(); }, price, vecSize, step,
                                  "_vibrato_heston_gamma_anti");
    Helper<double>::generateGreek(&o, &heston, [&](void) -> double { return ve.vanna(); }, price, vecSize, step,
                                  "_vibrato_heston_vanna_anti");
    Helper<double>::generateGreek(&o, &heston, [&](void) -> double { return ve.volga(); }, price, vecSize, step,
                                  "_vibrato_heston_volga_anti");

    std::cout << "***** Vibrato for vanilla (Heston) terminated!" << std::endl;

    return 0;
}
