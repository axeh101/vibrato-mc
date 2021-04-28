#include <iostream>
#include "../all.hpp"
#include "helper.hpp"

using namespace std;


int main() {

    std::cout << "***** Staring vibrato AD for vanilla options!" << std::endl;

    double maturity = 1;
    double strike = 100;
    double price = 50;
    double rate = .05;
    double vol = .2;
    int n = 25;
    int M = 50000;
    int Mz = 20;

    // Product definition
    VanillaOption<double> call(maturity, strike, OptionType::Put);

    // Black Scholes process definition
    State<double> initialState = {0.0, price};
    BlackScholesProcess<double> bs(initialState, rate, vol);

    // Pricing engines definition
    auto be = AnalyticalVanillaBS<double>(&call, &bs);
    auto ve = VibratoAD<double>(&call, &bs, n, M, Mz);

    size_t vecSize = 100;
    int step = 1;

    // Black Scholes
    Helper::generateGreek(&call, &bs, [&](void) -> double { return be.gamma(); }, price, vecSize, step,
                          "_analytic_gamma");
    Helper::generateGreek(&call, &bs, [&](void) -> double { return be.vanna(); }, price, vecSize, step,
                          "_analytic_vanna");
    Helper::generateGreek(&call, &bs, [&](void) -> double { return be.volga(); }, price, vecSize, step,
                          "_analytic_volga");


    ve.antithetic = false;
    Helper::generateGreek(&call, &bs, [&](void) -> double { return ve.gamma(); }, price, vecSize, step,
                          "_vibratoad_gamma");
    Helper::generateGreek(&call, &bs, [&](void) -> double { return ve.vanna(); }, price, vecSize, step,
                          "_vibratoad_vanna");
    Helper::generateGreek(&call, &bs, [&](void) -> double { return ve.volga(); }, price, vecSize, step,
                          "_vibratoad_volga");

    // Antithetic outputs
    ve.antithetic = true;
    Helper::generateGreek(&call, &bs, [&](void) -> double { return ve.gamma(); }, price, vecSize, step,
                          "_vibratoad_gamma_anti");
    Helper::generateGreek(&call, &bs, [&](void) -> double { return ve.vanna(); }, price, vecSize, step,
                          "_vibratoad_vanna_anti");
    Helper::generateGreek(&call, &bs, [&](void) -> double { return ve.volga(); }, price, vecSize, step,
                          "_vibratoad_volga_anti");


    std::cout << "***** Vibrato for vanilla options terminated!" << std::endl;

    return 0;
}
