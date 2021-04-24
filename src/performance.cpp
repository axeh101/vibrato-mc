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
    int Mz = 100;
    int M = 10000;

    // Product definition
    VanillaOption<double> callDigital(maturity, strike, OptionType::Call);

    // Black Scholes process definition
    State<double> initialState = {0.0, 100};
    BlackScholesProcess<double> bs(initialState, rate, vol);

    // Pricing engines definition
    auto bsEngine = AnalyticalVanillaBS<double>(&callDigital, &bs);
    auto vibratoEngine = VibratoBS<double>(&callDigital, &bs, n, M, Mz);

    // Convergence  tests (Vibrato vs Vibrato antithetic)
    auto *analyticValue = new Path<double>(1001);
    auto *vibratoConv = new Path<double>(1001);
    auto *vibratoAntiConv = new Path<double>(1001);
    bsEngine.calculate();
    double delta = bsEngine.delta();
    for (int i = 0; i < 101; ++i) {
        vibratoEngine.M += 100;
        vibratoEngine.antithetic = false;
        vibratoEngine.calculate();
        (*analyticValue)[i] = {(double) vibratoEngine.M, delta};
        (*vibratoConv)[i] = {(double) vibratoEngine.M, vibratoEngine.delta()};

        vibratoEngine.antithetic = true;
        vibratoEngine.calculate();
        (*vibratoAntiConv)[i] = {(double) vibratoEngine.M, vibratoEngine.delta()};
        std::cout << vibratoEngine.M << std::endl;
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