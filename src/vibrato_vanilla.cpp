#include <iostream>
#include "all.hpp"

using namespace std;

int main() {

	std::cout << "***** Staring vibrato for vanilla options !" << std::endl;

	const std::string destination = "src/python/datasets/";

	double maturity = 1;
	double strike = 100;
	double rate = .05;
	double vol = .2;
	int n = 25;
	int M = 100000;
	int Mz = 1;

	// Product definition
	VanillaOption<double> callVanilla(maturity, strike, OptionType::Call);

	// Black Scholes process definition
	State<double> initialState = { 0.0, 100 };
	BlackScholesProcess<double> bs(initialState, rate, vol);

	// Pricing engines definition
	auto bsEngine = AnalyticalVanillaBS<double>(&callVanilla, &bs);
	auto vibratoEngine = VibratoBS<double>(&callVanilla, &bs, n, M, Mz);

	int vecSize = 200;
	// Premium tests
	auto *vibratoPremium = new Path<double>(vecSize);
	auto *analyticPremium = new Path<double>(vecSize);

	// delta tests
	auto *vibratoDelta = new Path<double>(vecSize);
	auto *analyticDelta = new Path<double>(vecSize);
	auto *vibratoDeltaAntithetic = new Path<double>(vecSize);

	// vega tests
	auto *vibratoVega = new Path<double>(vecSize);
	auto *analyticVega = new Path<double>(vecSize);
	auto *vibratoVegaAntithetic = new Path<double>(vecSize);

	// rho tests
	auto *vibratoRho = new Path<double>(vecSize);
	auto *analyticRho = new Path<double>(vecSize);
	auto *vibratoRhoAntithetic = new Path<double>(vecSize);

	double price = 60;
	for (int i = 0; i < vecSize; ++i) {
		bs.initialState.value = price;
		bsEngine.calculate();
		(*analyticPremium)[i] = { price, bsEngine.premium() };
		(*analyticDelta)[i] = { price, bsEngine.delta() };
		(*analyticVega)[i] = { price, bsEngine.vega() };
		(*analyticRho)[i] = { price, bsEngine.rho() };

		vibratoEngine.antithetic = true;
		vibratoEngine.calculate();
		(*vibratoPremium)[i] = { price, vibratoEngine.premium() };

		(*vibratoVegaAntithetic)[i] = { price, vibratoEngine.vega() };
		(*vibratoDeltaAntithetic)[i] = { price, vibratoEngine.delta() };
		(*vibratoRhoAntithetic)[i] = { price, vibratoEngine.rho() };

		vibratoEngine.antithetic = false;
		vibratoEngine.calculate();
		(*vibratoDelta)[i] = { price, vibratoEngine.delta() };
		(*vibratoVega)[i] = { price, vibratoEngine.vega() };
		(*vibratoRho)[i] = { price, vibratoEngine.rho() };

		price += 0.5;
	}
	vect2csv(destination + "call_analytic_premium", *analyticPremium);
	vect2csv(destination + "call_vibrato_premium", *vibratoPremium);
	vect2csv(destination + "call_analytic_delta", *analyticDelta);
	vect2csv(destination + "call_analytic_vega", *analyticVega);
	vect2csv(destination + "call_analytic_rho", *analyticRho);

	vect2csv(destination + "call_vibrato_delta", *vibratoDelta);
	vect2csv(destination + "call_vibrato_delta_antithetic",
			*vibratoDeltaAntithetic);
	vect2csv(destination + "call_vibrato_vega", *vibratoVega);
	vect2csv(destination + "call_vibrato_vega_antithetic", *vibratoVegaAntithetic);
	vect2csv(destination + "call_vibrato_rho", *vibratoRho);
	vect2csv(destination + "call_vibrato_rho_antithetic",*vibratoRhoAntithetic);

	delete vibratoPremium;
	delete analyticPremium;
	delete analyticDelta;
	delete analyticRho;

	delete vibratoDelta;
	delete vibratoVega;
	delete vibratoRho;

	delete vibratoDeltaAntithetic;
	delete vibratoVegaAntithetic;
	delete vibratoRhoAntithetic;

	std::cout << "***** Vibrato for vanilla options terminated!" << std::endl;

	return 0;
}
