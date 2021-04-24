#include <iostream>
#include "all.hpp"

using namespace std;

int main() {

    std::cout << "***** Staring vibrato for digital options !" << std::endl;

    const std::string destination = "src/python/datasets/";

    double maturity = 1;
    double strike = 100;
    double rate = .05;
    double vol = .2;
    int n = 25;
    int M = 1000;
    int Mz = 10;

    // Product definition
    DigitalOption<double> callDigital(maturity, strike, OptionType::Call);

    // Black Scholes process definition
    State<double> initialState = {0.0, 100};
    BlackScholesProcess<double> bs(initialState, rate, vol);

    // Pricing engines definition
    auto bsEngine = AnalyticalDigitalBS<double>(&callDigital, &bs);
    auto vibratoEngine = VibratoBS<double>(&callDigital, &bs, n, M, Mz);

    // Premium tests
    auto *vibratoPremium = new Path<double>(201);
    auto *analyticPremium = new Path<double>(201);

    // delta tests
    auto *vibratoDelta = new Path<double>(201);
    auto *analyticDelta = new Path<double>(201);
    auto *vibratoDeltaAnti = new Path<double>(201);

    // vega tests
    auto *vibratoVega = new Path<double>(201);
    auto *analyticVega = new Path<double>(201);
    auto *vibratoVegaAnti = new Path<double>(201);

    // rho tests
    auto *vibratoRho = new Path<double>(201);
    auto *analyticRho = new Path<double>(201);
    auto *vibratoRhoAnti = new Path<double>(201);

    double price = 1;
    for (int i = 0; i < 201; ++i) {
        bs.initialState.value = price;
        bsEngine.calculate();
        (*analyticPremium)[i] = {price, bsEngine.premium()};
        (*analyticDelta)[i] = {price, bsEngine.delta()};
        (*analyticVega)[i] = {price, bsEngine.vega()};
        (*analyticRho)[i] = {price, bsEngine.rho()};

        vibratoEngine.antithetic = true;
        vibratoEngine.calculate();
        (*vibratoPremium)[i] = {price, vibratoEngine.premium()};

        (*vibratoVegaAnti)[i] = {price, vibratoEngine.vega()};
        (*vibratoDeltaAnti)[i] = {price, vibratoEngine.delta()};
        (*vibratoRhoAnti)[i] = {price, vibratoEngine.rho()};

        vibratoEngine.antithetic = false;
        vibratoEngine.calculate();
        (*vibratoDelta)[i] = {price, vibratoEngine.delta()};
        (*vibratoVega)[i] = {price, vibratoEngine.vega()};
        (*vibratoRho)[i] = {price, vibratoEngine.rho()};

        price += 1;
    }
    vect2csv(destination + "digicall_analytic_premium", *analyticPremium);
    vect2csv(destination + "digicall_vibrato_premium", *vibratoPremium);
    vect2csv(destination + "digicall_analytic_delta", *analyticDelta);
    vect2csv(destination + "digicall_analytic_vega", *analyticVega);
    vect2csv(destination + "digicall_analytic_rho", *analyticRho);

    vect2csv(destination + "digicall_vibrato_delta", *vibratoDelta);
    vect2csv(destination + "digicall_vibrato_delta_antithetic", *vibratoDeltaAnti);
    vect2csv(destination + "digicall_vibrato_vega", *vibratoVega);
    vect2csv(destination + "digicall_vibrato_vega_antithetic", *vibratoVegaAnti);
    vect2csv(destination + "digicall_vibrato_rho", *vibratoRho);
    vect2csv(destination + "digicall_vibrato_rho_antithetic", *vibratoRhoAnti);

    delete vibratoPremium;
    delete analyticPremium;
    delete analyticDelta;
    delete analyticRho;

    delete vibratoDelta;
    delete vibratoVega;
    delete vibratoRho;

    delete vibratoDeltaAnti;
    delete vibratoVegaAnti;
    delete vibratoRhoAnti;

    std::cout << "***** Vibrato for digital options Terminated!" << std::endl;

    return 0;
}
