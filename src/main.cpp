#ifndef MAIN_CPP
#define MAIN_CPP


#include "all.hpp"

using namespace std;
int main(void)
{

    // Normal distributions tests
    //
     NormalDistribution<> normal = NormalDistribution<>(0, 1);
     cout << normal.cdf(0) << endl;
     cout << normal.pdf(0) << endl;
     vector<double> v =  normal.generate(10000);

    // Analytical formulation for an European Option
    //

    double maturity = .5;
    double strike = 100;
    VanillaOption<double> callVanilla(maturity, strike, OptionType::Call);
    VanillaOption<double> putVanilla(maturity, strike, OptionType::Put);
    DigitalOption<double> callDigital(maturity, strike, OptionType::Call);
    cout << callVanilla << endl;
    cout << putVanilla << endl;
    cout << callDigital << endl;

    cout << "*****************************" << endl;
    State<double> initial_state = {0.0, 100};
    double rate= .02;
    double vol = .03;
    BlackScholesProcess<double> bs(initial_state, rate, vol);
    AnalyticalVanillaBS<double> bsEngineCall = AnalyticalVanillaBS<double>(&callVanilla, &bs);
    AnalyticalVanillaBS<double> bsEnginePut = AnalyticalVanillaBS<double>(&putVanilla, &bs);
    bsEngineCall.calculate();
    bsEnginePut.calculate();
    cout << bsEngineCall << endl;
    cout << bsEnginePut << endl;

    cout << "*****************************" << endl;
    cout << bs.eulerPriceDiff(.1) << endl;
    cout << bs.eulerPriceDiff(.3) << endl;
    cout << bs.eulerPriceDiff(.5) << endl;
    Path<double> path = bs.generatePath(1000, 1);
    vect2csv("src/python/datasets/bspath", path);

    Path<double> path2 = bs.eulerDiscretization(1000, 1);
    vect2csv("src/python/datasets/bseulerpath", path2);
    cout << "*****************************" << endl;
    cout << bs << endl;






    return 0;
}

#endif  // MAIN_CPP
