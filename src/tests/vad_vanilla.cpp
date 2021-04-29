#include <iostream>
#include "../all.hpp"
#include "helper.hpp"

#include <autodiff/forward.hpp>

using namespace autodiff;
using namespace std;


int main() {

    std::cout << "***** Staring vibrato AD for vanilla options!" << std::endl;

    dual maturity = 1;
    dual strike = 100;
    dual price = 50;

    dual rate = .05;
    dual vol = .2;

    int n = 100;
    int M = 10000;
    int Mz = 20;

    // Product definition
    VanillaOption<dual> o(maturity, strike, OptionType::Put);

    // Black Scholes process definition
    State<dual> initialState = {0.0, price};
    BlackScholesProcess<dual> bs(initialState, rate, vol);

    // Pricing engines definition
    auto be = AnalyticalVanillaBS<dual>(&o, &bs);
    auto ve = VibratoAD<dual>(&o, &bs, n, M, Mz);

    size_t vecSize = 100;
    int step = 1;

    // Black Scholes
    Helper<dual>::generateGreek(&o, &bs, [&](void) -> dual { return be.gamma(); }, price, vecSize, step,
                                "_analytic_ad_gamma");
    Helper<dual>::generateGreek(&o, &bs, [&](void) -> dual { return be.vanna(); }, price, vecSize, step,
                                "_analytic_ad_vanna");


    ve.antithetic = false;
    Helper<dual>::generateGreek(&o, &bs, [&](void) -> dual { return ve.gamma(); }, price, vecSize, step,
                                "_vibrato_ad_gamma");
    Helper<dual>::generateGreek(&o, &bs, [&](void) -> dual { return ve.vanna(); }, price, vecSize, step,
                                "_vibrato_ad_vanna");

    // Antithetic outputs
    ve.antithetic = true;
    Helper<dual>::generateGreek(&o, &bs, [&](void) -> dual { return ve.gamma(); }, price, vecSize, step,
                                "_vibrato_ad_gamma_anti");
    Helper<dual>::generateGreek(&o, &bs, [&](void) -> dual { return ve.vanna(); }, price, vecSize, step,
                                "_vibrato_ad_vanna_anti");


    std::cout << "***** Vibrato for vanilla options terminated!" << std::endl;

    return 0;
}
