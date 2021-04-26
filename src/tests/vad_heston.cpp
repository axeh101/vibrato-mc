#include <iostream>
#include "../all.hpp"

using namespace std;

int main() {

    std::cout << "***** Staring vibrato AD for options (Heston) !" << std::endl;

    const std::string destination = "src/python/datasets/";

    double maturity = 1;
    double strike = 90;
    double rate = .00135;
    int n = 100;
    int M = 100000;
    int Mz = 2;
    double vol = .2;

    // Heston params
    double initialVol = 0.028087;
    double correlation = .5;
    double kappa = 2.931465; // mean reversion
    double eta = 0.101; // long run mean
    double volvol = 0.01;

    // Product definition
    VanillaOption<double> call(maturity, strike, OptionType::Call);

    // Heston & CIR process definition
    State<double> initialState = {0.0, 60};
    CoxIngersollRossProcess<double> cir({0, initialVol}, kappa, eta, volvol);
    HestonProcess<double> heston(initialState, &cir, rate, correlation);
    BlackScholesProcess<double> bs(initialState, rate, vol);

    // Pricing engines definition
    auto vibratoEngine = VibratoAD<double>(&call, &bs, n, M, Mz);
    auto bsEngine = AnalyticalDigitalBS<double>(&call, &bs);

    int vecSize = 200;

    // gamma tests
    auto *vibratoGamma = new Path<double>(vecSize);
    auto *vibratoGammaAnti = new Path<double>(vecSize);

    // vanna tests
    auto *vibratoVanna = new Path<double>(vecSize);
    auto *vibratoVannaAnti = new Path<double>(vecSize);

    // volga tests
    auto *vibratoVolga = new Path<double>(vecSize);
    auto *vibratoVolgaAnti = new Path<double>(vecSize);


    double price = 60;
    for (int i = 0; i < vecSize; ++i) {
        bs.initialState.value = price;

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

//    vect2csv(destination + "call_analytic_heston_vanna", *analyticVanna);
    vect2csv(destination + "call_vibratoad_heston_vanna", *vibratoVanna);
    vect2csv(destination + "call_vibratoad_heston_vanna_antithetic", *vibratoVannaAnti);

//    vect2csv(destination + "call_analytic_heston_gamma", *analyticGamma);
    vect2csv(destination + "call_vibratoad_heston_gamma", *vibratoGamma);
    vect2csv(destination + "call_vibratoad_heston_gamma_antithetic", *vibratoGammaAnti);

//    vect2csv(destination + "call_analytic_heston_volga", *analyticVolga);
    vect2csv(destination + "call_vibratoad_heston_volga", *vibratoVolga);
    vect2csv(destination + "call_vibratoad_heston_volga_antithetic", *vibratoVolgaAnti);

    delete vibratoGamma;
    delete vibratoGammaAnti;
//    delete analyticGamma;

    delete vibratoVanna;
    delete vibratoVannaAnti;
//    delete analyticVanna;

    delete vibratoVolga;
    delete vibratoVolgaAnti;
//    delete analyticVolga;
    std::cout << "***** Vibrato  AD for vanilla (Heston) terminated!" << std::endl;

    return 0;
}
