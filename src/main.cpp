#ifndef MAIN_CPP
#define MAIN_CPP


#include "all.hpp"

using namespace std;
int main(void)
{

    // Normal distributions tests
    //
    // NormalDistribution<> normal = NormalDistribution<>(0, 1);
    // std::cout << normal.cdf(0) << std::endl;
    // std::cout << normal.pdf(0) << std::endl;
    // std::vector<double> v =  normal.generate(10000);
    // vect2csv("normalrv", v);

    // Analytical formulation for an European Option
    //
    OptionType type = OptionType::Call;
    double maturity = .5;
    double strike = 100;
    VanillaOption<double> callVanilla(maturity, strike, type);
    DigitalOption<double> callDigital(maturity, strike, type);
    cout << callVanilla << endl;
    cout << callDigital << endl;

    std::cout << "*****************************" << std::endl;
    State<double> initial_state = {0.0, 100};
    double rate= .02;
    double vol = .03;
    BlackScholesProcess<double> bs(initial_state, rate, vol);
    AnalyticalBS<double> bsEngine = AnalyticalBS<double>(callVanilla, bs);
    bsEngine.calculate();
    std::cout << bsEngine << std::endl;

    std::cout << "*****************************" << std::endl;
    Path<double> path = bs.generatePath(100, 1);
    vect2csv("python/datasets/bspath", path);

    std::cout << "*****************************" << std::endl;
    std::cout << bs << std::endl;






    return 0;
}

#endif  // MAIN_CPP
