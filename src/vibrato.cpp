#include <iostream>
#include "all.hpp"

using namespace std;
int main() {

    double maturity = .5;
    double strike = 100;
	double rate= .05;
	double vol = .3;
	int n = 100;
	int M = 1000;
	int Mz = 100;

	// Product definition
	VanillaOption<double> callVanilla(maturity, strike, OptionType::Call);
	VanillaOption<double> putVanilla(maturity, strike, OptionType::Put);
	DigitalOption<double> callDigital(maturity, strike, OptionType::Call);
	DigitalOption<double> putDigital(maturity, strike, OptionType::Put);

	// Black Scholes process definition
	State<double> initialState = {0.0, 100};
	BlackScholesProcess<double> bs(initialState, rate, vol);

	AnalyticalBS<double> bsEngine = AnalyticalBS<double>(&callVanilla, &bs);
	VibratoBS<double> vibratoEngine = VibratoBS<double>(&callVanilla, &bs, n, M, Mz);


	// Premium tests
	Path<double>* vibratoPremium = new Path<double>(201);
	Path<double>* analyticPremium = new Path<double>(201);

	double price = 60;
	for (int i = 0; i < 201; ++i) {
		bs.initialState.value = price;
		bsEngine.calculate();
		vibratoEngine.calculate();
		(*vibratoPremium)[i] = {price, vibratoEngine.premium()};
		(*analyticPremium)[i] = {price, bsEngine.premium()};
		price += 0.5;
	}
	vect2csv("src/python/datasets/call_analytic_premium", *analyticPremium);
	vect2csv("src/python/datasets/call_vibrato_premium", *analyticPremium);


	bsEngine.changeOption(&putVanilla);
	vibratoEngine.changeOption(&putVanilla);

	price = 60;
	for (int i = 0; i < 201; ++i) {
		bs.initialState.value = price;
		bsEngine.calculate();
		vibratoEngine.calculate();
		(*vibratoPremium)[i] = {price, vibratoEngine.premium()};
		(*analyticPremium)[i] = {price, bsEngine.premium()};
		price += 0.5;
	}
	vect2csv("src/python/datasets/put_analytic_premium", *analyticPremium);
	vect2csv("src/python/datasets/put_vibrato_premium", *analyticPremium);


	delete vibratoPremium;
	delete analyticPremium;

	return 0;
}
