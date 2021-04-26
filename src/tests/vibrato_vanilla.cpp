#include <iostream>
#include "../all.hpp"

using namespace std;

int main() {

    std::cout << "***** Staring vibrato for vanilla options !" << std::endl;

    const std::string destination = "src/python/datasets/";

    double maturity = .5;
    double strike = 100;
    double rate = .05;
    double vol = .2;
    int n = 25;
    int M = 100000;
    int Mz = 1;

    // Product definition
    VanillaOption<double> callVanilla(maturity, strike, OptionType::Call);

    // Black Scholes process definition
    State<double> initialState = {0.0, 100};
    BlackScholesProcess<double> bs(initialState, rate, vol);

    // Pricing engines definition
    auto bsEngine = AnalyticalVanillaBS<double>(&callVanilla, &bs);
    auto vibratoEngine = Vibrato<double>(&callVanilla, &bs, n, M, Mz);

    int vecSize = 100;
    // Premium tests
    auto *vibratoPremium = new Path<double>(vecSize);
    auto *analyticPremium = new Path<double>(vecSize);

    // delta tests
    auto *vibratoDelta = new Path<double>(vecSize);
    auto *analyticDelta = new Path<double>(vecSize);
    auto *vibratoDeltaAnti = new Path<double>(vecSize);

    // vega tests
    auto *vibratoVega = new Path<double>(vecSize);
    auto *analyticVega = new Path<double>(vecSize);
    auto *vibratoVegaAnti = new Path<double>(vecSize);

    // rho tests
    auto *vibratoRho = new Path<double>(vecSize);
    auto *analyticRho = new Path<double>(vecSize);
    auto *vibratoRhoAnti = new Path<double>(vecSize);

    // rho tests
    auto *vibratoTheta = new Path<double>(vecSize);
    auto *analyticTheta = new Path<double>(vecSize);
    auto *vibratoThetaAnti = new Path<double>(vecSize);

    // gamma tests
    auto *vibratoGamma = new Path<double>(vecSize);
    auto *analyticGamma = new Path<double>(vecSize);
    auto *vibratoGammaAnti = new Path<double>(vecSize);

    // vanna tests
    auto *vibratoVanna = new Path<double>(vecSize);
    auto *analyticVanna = new Path<double>(vecSize);
    auto *vibratoVannaAnti = new Path<double>(vecSize);

    double price = 60;
    for (int i = 0; i < vecSize; ++i) {
        bs.initialState.value = price;
        (*analyticPremium)[i] = {price, bsEngine.premium()};
        (*analyticDelta)[i] = {price, bsEngine.delta()};
        (*analyticVega)[i] = {price, bsEngine.vega()};
        (*analyticRho)[i] = {price, bsEngine.rho()};
        (*analyticTheta)[i] = {price, bsEngine.theta()};
        (*analyticGamma)[i] = {price, bsEngine.gamma()};
        (*analyticVanna)[i] = {price, bsEngine.vanna()};

        vibratoEngine.antithetic = true;
        (*vibratoPremium)[i] = {price, vibratoEngine.premium()};
        (*vibratoVegaAnti)[i] = {price, vibratoEngine.vega()};
        (*vibratoDeltaAnti)[i] = {price, vibratoEngine.delta()};
        (*vibratoRhoAnti)[i] = {price, vibratoEngine.rho()};
        (*vibratoThetaAnti)[i] = {price, vibratoEngine.theta()};
        (*vibratoGammaAnti)[i] = {price, vibratoEngine.gamma()};
        (*vibratoVannaAnti)[i] = {price, vibratoEngine.vanna()};

        vibratoEngine.antithetic = false;
        (*vibratoDelta)[i] = {price, vibratoEngine.delta()};
        (*vibratoVega)[i] = {price, vibratoEngine.vega()};
        (*vibratoRho)[i] = {price, vibratoEngine.rho()};
        (*vibratoTheta)[i] = {price, vibratoEngine.theta()};
        (*vibratoGamma)[i] = {price, vibratoEngine.gamma()};
        (*vibratoVanna)[i] = {price, vibratoEngine.vanna()};


        price += 1;
    }

    vect2csv(destination + "call_analytic_premium", *analyticPremium);
    vect2csv(destination + "call_vibrato_premium", *vibratoPremium);


    vect2csv(destination + "call_analytic_delta", *analyticDelta);
    vect2csv(destination + "call_vibrato_delta", *vibratoDelta);
    vect2csv(destination + "call_vibrato_delta_antithetic", *vibratoDeltaAnti);

    vect2csv(destination + "call_analytic_vega", *analyticVega);
    vect2csv(destination + "call_vibrato_vega", *vibratoVega);
    vect2csv(destination + "call_vibrato_vega_antithetic", *vibratoVegaAnti);

    vect2csv(destination + "call_analytic_rho", *analyticRho);
    vect2csv(destination + "call_vibrato_rho", *vibratoRho);
    vect2csv(destination + "call_vibrato_rho_antithetic", *vibratoRhoAnti);
//
    vect2csv(destination + "call_analytic_vanna", *analyticVanna);
    vect2csv(destination + "call_vibrato_vanna", *vibratoVanna);
    vect2csv(destination + "call_vibrato_vanna_antithetic", *vibratoVannaAnti);

    vect2csv(destination + "call_analytic_gamma", *analyticGamma);
    vect2csv(destination + "call_vibrato_gamma", *vibratoGamma);
    vect2csv(destination + "call_vibrato_gamma_antithetic", *vibratoGammaAnti);

    vect2csv(destination + "call_analytic_theta", *analyticTheta);
    vect2csv(destination + "call_vibrato_theta", *vibratoTheta);
    vect2csv(destination + "call_vibrato_theta_antithetic", *vibratoThetaAnti);

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

    delete vibratoTheta;
    delete vibratoThetaAnti;
    delete analyticTheta;

    delete vibratoGamma;
    delete vibratoGammaAnti;
    delete analyticGamma;

    delete vibratoVanna;
    delete vibratoVannaAnti;
    delete analyticVanna;

    std::cout << "***** Vibrato for vanilla options terminated!" << std::endl;

    return 0;
}
