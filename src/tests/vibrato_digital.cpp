#include <iostream>
#include "../all.hpp"
#include "helper.hpp"

using namespace std;

int main() {

    std::cout << "***** Staring vibrato for digital options !" << std::endl;

    double maturity = 1;
    double strike = 100;
    double price = 50;
    double rate = .05;
    double vol = .2;
    int n = 25;
    int M = 10000;
    int Mz = 20;

    // Product definition
    DigitalOption<double> o(maturity, strike, OptionType::Call);

    // Black Scholes process definition
    State<double> initialState = {0.0, price};
    BlackScholesProcess<double> bs(initialState, rate, vol);

    // Pricing engines definition
    auto be = AnalyticalDigitalBS<double>(&o, &bs);
    auto ve = Vibrato<double>(&o, &bs, n, M, Mz);

    // Product definition

    size_t vecSize = 100;
    int step = 1;


    // Black Scholes
    Helper<double>::generateGreek(&o, &bs, [&](void) -> double { return be.premium(); }, price, vecSize, step,
                                  "_analytic_digital_premium");

    // First Order
    Helper<double>::generateGreek(&o, &bs, [&](void) -> double { return be.delta(); }, price, vecSize, step,
                                  "_analytic_digital_delta");
    Helper<double>::generateGreek(&o, &bs, [&](void) -> double { return be.vega(); }, price, vecSize, step,
                                  "_analytic_digital_vega");
    Helper<double>::generateGreek(&o, &bs, [&](void) -> double { return be.rho(); }, price, vecSize, step,
                                  "_analytic_digital_rho");

    // Second order greeks
    Helper<double>::generateGreek(&o, &bs, [&](void) -> double { return be.gamma(); }, price, vecSize, step,
                                  "_analytic_digital_gamma");
    Helper<double>::generateGreek(&o, &bs, [&](void) -> double { return be.vanna(); }, price, vecSize, step,
                                  "_analytic_digital_vanna");
    Helper<double>::generateGreek(&o, &bs, [&](void) -> double { return be.volga(); }, price, vecSize, step,
                                  "_analytic_digital_volga");

    // Vibrato
    Helper<double>::generateGreek(&o, &bs, [&](void) -> double { return ve.premium(); }, price, vecSize, step,
                                  "_vibrato_digital_premium");

    ve.antithetic = false;
    // First order greeks
    Helper<double>::generateGreek(&o, &bs, [&](void) -> double { return ve.delta(); }, price, vecSize, step,
                                  "_vibrato_digital_delta");
    Helper<double>::generateGreek(&o, &bs, [&](void) -> double { return ve.vega(); }, price, vecSize, step,
                                  "_vibrato_digital_vega");
    Helper<double>::generateGreek(&o, &bs, [&](void) -> double { return ve.rho(); }, price, vecSize, step,
                                  "_vibrato_digital_rho");
    // Second order greeks
    Helper<double>::generateGreek(&o, &bs, [&](void) -> double { return ve.gamma(); }, price, vecSize, step,
                                  "_vibrato_digital_gamma");
    Helper<double>::generateGreek(&o, &bs, [&](void) -> double { return ve.vanna(); }, price, vecSize, step,
                                  "_vibrato_digital_vanna");
    Helper<double>::generateGreek(&o, &bs, [&](void) -> double { return ve.volga(); }, price, vecSize, step,
                                  "_vibrato_digital_volga");


    // Antithetic outos
    ve.antithetic = true;
    // First order greeks
    Helper<double>::generateGreek(&o, &bs, [&](void) -> double { return ve.delta(); }, price, vecSize, step,
                                  "_vibrato_digital_delta_anti");
    Helper<double>::generateGreek(&o, &bs, [&](void) -> double { return ve.vega(); }, price, vecSize, step,
                                  "_vibrato_digital_vega_anti");
    Helper<double>::generateGreek(&o, &bs, [&](void) -> double { return ve.rho(); }, price, vecSize, step,
                                  "_vibrato_digital_rho_anti");
    // Second order greeks
    Helper<double>::generateGreek(&o, &bs, [&](void) -> double { return ve.gamma(); }, price, vecSize, step,
                                  "_vibrato_digital_gamma_anti");
    Helper<double>::generateGreek(&o, &bs, [&](void) -> double { return ve.vanna(); }, price, vecSize, step,
                                  "_vibrato_digital_vanna_anti");
    Helper<double>::generateGreek(&o, &bs, [&](void) -> double { return ve.volga(); }, price, vecSize, step,
                                  "_vibrato_digital_volga_anti");

    std::cout << "***** Vibrato for digital options terminated!" << std::endl;

    return 0;
}
