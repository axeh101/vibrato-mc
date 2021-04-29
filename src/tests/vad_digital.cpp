#include <iostream>
#include "../all.hpp"
#include "helper.hpp"

using namespace std;


int main() {

    std::cout << "***** Staring vibrato AD for digital options !" << std::endl;

    dual maturity = 1;
    dual strike = 100;
    dual price = 50;

    dual rate = .05;
    dual vol = .2;

    int n = 25;
    int M = 50000;
    int Mz = 10;

    // Product definition
    DigitalOption<dual> o(maturity, strike, OptionType::Put);

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
                                "_analytic_digital_ad_gamma");
    Helper<dual>::generateGreek(&o, &bs, [&](void) -> dual { return be.vanna(); }, price, vecSize, step,
                                "_analytic_digital_ad_vanna");
    Helper<dual>::generateGreek(&o, &bs, [&](void) -> dual { return be.volga(); }, price, vecSize, step,
                                "_analytic_digital_ad_volga");


    ve.antithetic = false;
    Helper<dual>::generateGreek(&o, &bs, [&](void) -> dual { return ve.gamma(); }, price, vecSize, step,
                                "_vibrato_digital_ad_gamma");
    Helper<dual>::generateGreek(&o, &bs, [&](void) -> dual { return ve.vanna(); }, price, vecSize, step,
                                "_vibrato_digital_ad_vanna");
    Helper<dual>::generateGreek(&o, &bs, [&](void) -> dual { return ve.volga(); }, price, vecSize, step,
                                "_vibrato_digital_ad_volga");

    // Antithetic outputs
    ve.antithetic = true;
    Helper<dual>::generateGreek(&o, &bs, [&](void) -> dual { return ve.gamma(); }, price, vecSize, step,
                                "_vibrato_digital_ad_gamma_anti");
    Helper<dual>::generateGreek(&o, &bs, [&](void) -> dual { return ve.vanna(); }, price, vecSize, step,
                                "_vibrato_digital_ad_vanna_anti");
    Helper<dual>::generateGreek(&o, &bs, [&](void) -> dual { return ve.volga(); }, price, vecSize, step,
                                "_vibrato_digital_ad_volga_anti");


    std::cout << "***** Vibrato AD for digital options terminated!" << std::endl;

    return 0;
}
