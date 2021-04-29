#include <iostream>
#include "../all.hpp"
#include "helper.hpp"

using namespace std;


int main() {

    std::cout << "***** Staring vibrato AD for digital options !" << std::endl;

    double maturity = 1;
    double strike = 100;
    double price = 50;

    double rate = .05;
    double vol = .2;

    int n = 25;
    int M = 50000;
    int Mz = 10;

    // Product definition
    DigitalOption<double> o(maturity, strike, OptionType::Put);

    // Black Scholes process definition
    State<double> initialState = {0.0, price};
    BlackScholesProcess<double> bs(initialState, rate, vol);

    // Pricing engines definition
    auto be = AnalyticalVanillaBS<double>(&o, &bs);
    auto ve = VibratoAD<double>(&o, &bs, n, M, Mz);

    size_t vecSize = 100;
    int step = 1;

    // Black Scholes
    Helper::generateGreek(&o, &bs, [&](void) -> double { return be.gamma(); }, price, vecSize, step,
                          "_analytic_digital_ad_gamma");
    Helper::generateGreek(&o, &bs, [&](void) -> double { return be.vanna(); }, price, vecSize, step,
                          "_analytic_digital_ad_vanna");
    Helper::generateGreek(&o, &bs, [&](void) -> double { return be.volga(); }, price, vecSize, step,
                          "_analytic_digital_ad_volga");


    ve.antithetic = false;
    Helper::generateGreek(&o, &bs, [&](void) -> double { return ve.gamma(); }, price, vecSize, step,
                          "_vibrato_digital_ad_gamma");
    Helper::generateGreek(&o, &bs, [&](void) -> double { return ve.vanna(); }, price, vecSize, step,
                          "_vibrato_digital_ad_vanna");
    Helper::generateGreek(&o, &bs, [&](void) -> double { return ve.volga(); }, price, vecSize, step,
                          "_vibrato_digital_ad_volga");

    // Antithetic outputs
    ve.antithetic = true;
    Helper::generateGreek(&o, &bs, [&](void) -> double { return ve.gamma(); }, price, vecSize, step,
                          "_vibrato_digital_ad_gamma_anti");
    Helper::generateGreek(&o, &bs, [&](void) -> double { return ve.vanna(); }, price, vecSize, step,
                          "_vibrato_digital_ad_vanna_anti");
    Helper::generateGreek(&o, &bs, [&](void) -> double { return ve.volga(); }, price, vecSize, step,
                          "_vibrato_digital_ad_volga_anti");


    std::cout << "***** Vibrato AD for digital options terminated!" << std::endl;

    return 0;
}
