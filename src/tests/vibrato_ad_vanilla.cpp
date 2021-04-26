#include <iostream>
#include "../all.hpp"

using namespace std;


int main() {

    std::cout << "***** Staring vibrato AD for vanilla options!" << std::endl;

    const std::string destination = "src/python/datasets/";

    double maturity = .5;
    double strike = 100;
    double rate = .05;
    double vol = .2;
    int n = 25;
    int M = 1000;
    int Mz = 10;

    // Product definition
    VanillaOption<double> call(maturity, strike, OptionType::Call);

    // Black Scholes process definition
    State<double> initialState = {0.0, 100};
    BlackScholesProcess<double> bs(initialState, rate, vol);

    // Pricing engines definition
    auto bsEngine = AnalyticalVanillaBS<double>(&call, &bs);
    auto vibratoEngine = VibratoAD<double>(&call, &bs, n, M, Mz);

    int vecSize = 200;

    // gamma tests
    auto *vibratoGamma = new Path<double>(vecSize);
    auto *analyticGamma = new Path<double>(vecSize);
    auto *vibratoGammaAnti = new Path<double>(vecSize);

    // vanna tests
    auto *vibratoVanna = new Path<double>(vecSize);
    auto *analyticVanna = new Path<double>(vecSize);
    auto *vibratoVannaAnti = new Path<double>(vecSize);

    // volga tests
    auto *vibratoVolga = new Path<double>(vecSize);
    auto *analyticVolga = new Path<double>(vecSize);
    auto *vibratoVolgaAnti = new Path<double>(vecSize);


    double price = 60;
    for (int i = 0; i < vecSize; ++i) {
        bs.initialState.value = price;
        bsEngine.calculate();

        (*analyticGamma)[i] = {price, bsEngine.gamma()};
        (*analyticVanna)[i] = {price, bsEngine.vanna()};
        (*analyticVolga)[i] = {price, bsEngine.volga()};

        vibratoEngine.antithetic = true;
        (*vibratoGammaAnti)[i] = {price, vibratoEngine.gamma()};
        (*vibratoVannaAnti)[i] = {price, vibratoEngine.vanna()};
        (*vibratoVolgaAnti)[i] = {price, vibratoEngine.volga()};

        vibratoEngine.antithetic = false;
        (*vibratoGamma)[i] = {price, vibratoEngine.gamma()};
        (*vibratoVanna)[i] = {price, vibratoEngine.vanna()};
        (*vibratoVolga)[i] = {price, vibratoEngine.volga()};

        price += 0.5;
    }

    vect2csv(destination + "call_analytic_vanna", *analyticVanna);
    vect2csv(destination + "call_vibratoad_vanna", *vibratoVanna);
    vect2csv(destination + "call_vibratoad_vanna_antithetic", *vibratoVannaAnti);

    vect2csv(destination + "call_analytic_gamma", *analyticGamma);
    vect2csv(destination + "call_vibratoad_gamma", *vibratoGamma);
    vect2csv(destination + "call_vibratoad_gamma_antithetic", *vibratoGammaAnti);

    vect2csv(destination + "call_analytic_volga", *analyticVolga);
    vect2csv(destination + "call_vibratoad_volga", *vibratoVolga);
    vect2csv(destination + "call_vibratoad_volga_antithetic", *vibratoVolgaAnti);

    delete vibratoGamma;
    delete vibratoGammaAnti;
    delete analyticGamma;

    delete vibratoVanna;
    delete vibratoVannaAnti;
    delete analyticVanna;

    std::cout << "***** Vibrato for vanilla options terminated!" << std::endl;

    return 0;
}