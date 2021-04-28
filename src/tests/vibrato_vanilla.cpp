#include <iostream>
#include "../all.hpp"
#include "helper.hpp"

using namespace std;

int main() {

    std::cout << "***** Staring vibrato for vanilla options !" << std::endl;

    double maturity = 1;
    double strike = 100;
    double rate = .05;
    double vol = .2;
    int n = 25;
    int M = 50000;
    int Mz = 20;
    double price = 50;

    // Product definition
    VanillaOption<double> o(maturity, strike, OptionType::Put);

    // Black Scholes process definition
    State<double> initialState = {0.0, price};
    BlackScholesProcess<double> bs(initialState, rate, vol);

    // Pricing engines definition
    auto be = AnalyticalVanillaBS<double>(&o, &bs);
    auto ve = Vibrato<double>(&o, &bs, n, M, Mz);

    size_t vecSize = 100;
    int step = 1;
    // Black Scholes
    Helper::generateGreek(&o, &bs, [&](void) -> double { return be.premium(); }, price, vecSize, step,
                          "_analytic_premium");

    Helper::generateGreek(&o, &bs, [&](void) -> double { return be.delta(); }, price, vecSize, step,
                          "_analytic_delta");
    Helper::generateGreek(&o, &bs, [&](void) -> double { return be.vega(); }, price, vecSize, step,
                          "_analytic_vega");
    Helper::generateGreek(&o, &bs, [&](void) -> double { return be.rho(); }, price, vecSize, step,
                          "_analytic_rho");
    // Second order greeks
    Helper::generateGreek(&o, &bs, [&](void) -> double { return be.gamma(); }, price, vecSize, step,
                          "_analytic_gamma");
    Helper::generateGreek(&o, &bs, [&](void) -> double { return be.vanna(); }, price, vecSize, step,
                          "_analytic_vanna");
    Helper::generateGreek(&o, &bs, [&](void) -> double { return be.volga(); }, price, vecSize, step,
                          "_analytic_volga");

    // Vibrato
    Helper::generateGreek(&o, &bs, [&](void) -> double { return ve.premium(); }, price, vecSize, step,
                          "_vibrato_premium");

    ve.antithetic = false;
    // First order greeks
    Helper::generateGreek(&o, &bs, [&](void) -> double { return ve.delta(); }, price, vecSize, step,
                          "_vibrato_delta");
    Helper::generateGreek(&o, &bs, [&](void) -> double { return ve.vega(); }, price, vecSize, step,
                          "_vibrato_vega");
    Helper::generateGreek(&o, &bs, [&](void) -> double { return ve.rho(); }, price, vecSize, step,
                          "_vibrato_rho");
    // Second order greeks
    Helper::generateGreek(&o, &bs, [&](void) -> double { return ve.gamma(); }, price, vecSize, step,
                          "_vibrato_gamma");
    Helper::generateGreek(&o, &bs, [&](void) -> double { return ve.vanna(); }, price, vecSize, step,
                          "_vibrato_vanna");
    Helper::generateGreek(&o, &bs, [&](void) -> double { return ve.volga(); }, price, vecSize, step,
                          "_vibrato_volga");


    // Antithetic outputs
    ve.antithetic = true;
    // First order greeks
    Helper::generateGreek(&o, &bs, [&](void) -> double { return ve.delta(); }, price, vecSize, step,
                          "_vibrato_delta_anti");
    Helper::generateGreek(&o, &bs, [&](void) -> double { return ve.vega(); }, price, vecSize, step,
                          "_vibrato_vega_anti");
    Helper::generateGreek(&o, &bs, [&](void) -> double { return ve.rho(); }, price, vecSize, step,
                          "_vibrato_rho_anti");
    // Second order greeks
    Helper::generateGreek(&o, &bs, [&](void) -> double { return ve.gamma(); }, price, vecSize, step,
                          "_vibrato_gamma_anti");
    Helper::generateGreek(&o, &bs, [&](void) -> double { return ve.vanna(); }, price, vecSize, step,
                          "_vibrato_vanna_anti");
    Helper::generateGreek(&o, &bs, [&](void) -> double { return ve.volga(); }, price, vecSize, step,
                          "_vibrato_volga_anti");

    std::cout << "***** Vibrato for vanilla options terminated!" << std::endl;

    return 0;
}
