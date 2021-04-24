#include <iostream>
#include "all.hpp"

using namespace std;

int main() {

    std::cout << "***** Staring Performance tests !" << std::endl;

    const std::string destination = "src/python/datasets/";

    double maturity = 1;
    double strike = 100;
    double rate = .05;
    double vol = .2;
    int n = 25;
    int Mz = 1;
    int M = 100;

    // Product definition
    VanillaOption<double> callDigital(maturity, strike, OptionType::Call);

    // Black Scholes process definition
    State<double> initialState = {0.0, 80};
    BlackScholesProcess<double> bs(initialState, rate, vol);

    // Pricing engines definition
    auto bsEngine = AnalyticalVanillaBS<double>(&callDigital, &bs);
    auto vibratoEngine = VibratoBS<double>(&callDigital, &bs, n, M, Mz);

    // Convergence  tests (Vibrato vs Vibrato antithetic)
    int vecSize = 1000;
    auto *analyticValue = new Path<double>(vecSize);
    auto *vibratoConv = new Path<double>(vecSize);
    auto *vibratoAntiConv = new Path<double>(vecSize);

    bsEngine.calculate();
    double delta = bsEngine.delta();
    for (int i = 1; i < vecSize + 1; ++i) {
        vibratoEngine.antithetic = false;
        vibratoEngine.calculate();
        (*analyticValue)[i-1] = {(double) M*i, delta};
        (*vibratoConv)[i-1] = {(double) M*i, vibratoEngine.delta()};

        vibratoEngine.antithetic = true;
        vibratoEngine.calculate();
        (*vibratoAntiConv)[i-1] = {(double) M*i, vibratoEngine.delta()};
    }
    vect2csv(destination + "perf_delta_analytic", *analyticValue);
    vect2csv(destination + "perf_delta_vibrato", *vibratoConv);
    vect2csv(destination + "perf_delta_vibrato_anti", *vibratoAntiConv);


    delete analyticValue;
    delete vibratoConv;
    delete vibratoAntiConv;


    std::cout << "***** Performance tests terminated!" << std::endl;
    return 0;
}