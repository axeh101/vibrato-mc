#ifndef MAIN_CPP
#define MAIN_CPP


#include "../all.hpp"

using namespace std;

int main(void) {

    // Computation of the premium and greeks analytical BS vs vibrato BS

    double maturity = 1;
    double strike = 100;
    double price = 100;
    double rate = .05;
    double vol = .32;
    int n = 100;
    int M = 10000;
    int Mz = 20;


    // Heston params
    double initialVol = 0.028087;
    double correlation = .5;
    double kappa = 2.931465; // mean reversion
    double eta = 0.101; // long run mean
    double volvol = 0.05;

    // Heston & CIR process definition
    State<double> initialState = {0.0, price};
    CoxIngersollRossProcess<double> cir({0, initialVol}, kappa, eta, volvol);
    HestonProcess<double> heston(initialState, &cir, rate, correlation);
    BlackScholesProcess<double> bs(initialState, rate, vol);

    VanillaOption<double> vcall(maturity, strike, OptionType::Call);
    DigitalOption<double> dput(maturity, strike, OptionType::Put);

    // Pricing engines definition
    auto ve = Vibrato<double>(&vcall, &heston, n, M, Mz);
    auto be = AnalyticalVanillaBS<double>(&vcall, &bs);
    auto de = AnalyticalDigitalBS<double>(&dput, &bs);


    cout << "**************** Pricing results ****************" << endl;

    cout << "*************************************************" << endl;
    cout << "Pricing Vanilla option Analytic vs Vibrato Heston" << endl;
    cout << "*************************************************" << endl;

    cout << "*************** Call Black-Scholes **************" << endl;
    cout << be << endl;
    cout << "************* Call Vibrato (Heston) *************" << endl;
    cout << ve << endl;

    cout << "*************************************************" << endl;
    cout << "Pricing Digital option Analytic vs Vibrato Heston" << endl;
    cout << "*************************************************" << endl;

    ve.changeOption(dput);
    bs.resetState();

    cout << "*************** Put Black-Scholes ***************" << endl;
    cout << de << endl;
    cout << "****************** Vibrato (BS) *****************" << endl;
    cout << ve << endl;
    cout << "*********************~END~***********************" << endl;

    return 0;
}

#endif  // MAIN_CPP
