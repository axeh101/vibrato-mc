#ifndef MAIN_CPP
#define MAIN_CPP


#include "../all.hpp"

using namespace std;

int main(void) {

    // Computation of the premium and greeks analytical BS vs vibrato BS

    double maturity = 1;
    double strike = 90;
    double rate = .00135;
    int n = 100;
    int M = 10000;
    int Mz = 2;
    double vol = .2;

    double price = 60;

    VanillaOption<double> vcall(maturity, strike, OptionType::Call);
    DigitalOption<double> dput(maturity, strike, OptionType::Call);



    cout << "***********Pricing results**********" << endl;
    State<double> initialState = {0.0, price};
    // Black Scholes
    BlackScholesProcess<double> bs(initialState, rate, vol);



    auto bsEngine = AnalyticalVanillaBS<double>(&vcall, &bs);
    auto vbEngine = Vibrato<double>(&vcall, &bs, n, M, Mz);
    bsEngine.calculate();
    vbEngine.calculate();
    cout << "Call Black-Scholes **************" << endl;
    cout << bsEngine << endl;
    cout << "Call Vibrato ********************" << endl;
    cout << vbEngine << endl;
    cout << "*********************************" << endl;

    bsEngine.changeOption(dput);
    vbEngine.changeProcess(bs);
    vbEngine.changeOption(dput);
    vbEngine.calculate();
    bsEngine.calculate();
    bs.resetState();

    cout << "Put Black-Scholes **************" << endl;
    cout << bsEngine << endl;
    cout << "Put Vibrato ********************" << endl;
    cout << vbEngine << endl;
    cout << "**************~END~**************" << endl;

    return 0;
}

#endif  // MAIN_CPP
