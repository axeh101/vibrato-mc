#include <iostream>
#include "../all.hpp"
#include "helper.hpp"

using namespace std;

int main() {

    std::cout << "***** Staring vibrato AD for options (Heston) !" << std::endl;

    dual maturity = 1;
    dual strike = 100;
    dual price = 50;
    dual rate = .05;
    dual vol = .32;
    int n = 100;
    int M = 10000;
    int Mz = 20;


    // Heston params
    dual initialVol = 0.028087;
    dual correlation = .5;
    dual kappa = 2.931465; // mean reversion
    dual eta = 0.101; // long run mean
    dual volvol = 0.05;

    // Product definition
    VanillaOption<dual> o(maturity, strike, OptionType::Call);

    // Heston & CIR process definition
    State<dual> initialState = {0.0, price};
    CoxIngersollRossProcess<dual> cir({0, initialVol}, kappa, eta, volvol);
    HestonProcess<dual> heston(initialState, &cir, rate, correlation);
    BlackScholesProcess<dual> bs(initialState, rate, vol);

    // Pricing engines definition
    auto ve = VibratoAD<dual>(&o, &heston, n, M, Mz);
    auto be = AnalyticalVanillaBS<dual>(&o, &bs);

    size_t vecSize = 100;
    int step = 1;

    // Black Scholes

    Helper<dual>::generateGreek(&o, &bs, [&](void) -> dual { return be.gamma(); }, price, vecSize, step,
                                "_analytic_h_gamma");
    Helper<dual>::generateGreek(&o, &bs, [&](void) -> dual { return be.vanna(); }, price, vecSize, step,
                                "_analytic_h_vanna");

    ve.antithetic = false;
    Helper<dual>::generateGreek(&o, &heston, [&](void) -> dual { return ve.gamma(); }, price, vecSize, step,
                                "_vibrato_heston_ad_gamma");
    Helper<dual>::generateGreek(&o, &heston, [&](void) -> dual { return ve.vanna(); }, price, vecSize, step,
                                "_vibrato_heston_ad_vanna");

    // Antithetic outputs
    ve.antithetic = true;
    Helper<dual>::generateGreek(&o, &heston, [&](void) -> dual { return ve.gamma(); }, price, vecSize, step,
                                "_vibrato_heston_ad_gamma_anti");
    Helper<dual>::generateGreek(&o, &heston, [&](void) -> dual { return ve.vanna(); }, price, vecSize, step,
                                "_vibrato_heston_ad_vanna_anti");


    std::cout << "***** Vibrato  AD for vanilla (Heston) terminated!" << std::endl;

    return 0;
}
