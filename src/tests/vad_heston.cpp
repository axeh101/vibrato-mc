#include <iostream>
#include "../all.hpp"
#include "helper.hpp"
using namespace std;

int main() {

    std::cout << "***** Staring vibrato AD for options (Heston) !" << std::endl;

    const std::string destination = "src/python/datasets/";

    double maturity = 1;
    double strike = 100;
    double price = 50;
    double rate = .05;
    double vol = .2;
    int n = 25;
    int M = 50000;
    int Mz = 20;

    // Heston params
    double initialVol = 0.028087;
    double correlation = .5;
    double kappa = 2.931465; // mean reversion
    double eta = 0.101; // long run mean
    double volvol = 0.01;

    // Product definition
    VanillaOption<double> call(maturity, strike, OptionType::Call);

    // Heston & CIR process definition
    State<double> initialState = {0.0, price};
    CoxIngersollRossProcess<double> cir({0, initialVol}, kappa, eta, volvol);
    HestonProcess<double> heston(initialState, &cir, rate, correlation);
    BlackScholesProcess<double> bs(initialState, rate, vol);

    // Pricing engines definition
    auto ve = VibratoAD<double>(&call, &bs, n, M, Mz);
    auto be = AnalyticalDigitalBS<double>(&call, &bs);

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


    std::cout << "***** Vibrato  AD for vanilla (Heston) terminated!" << std::endl;

    return 0;
}
