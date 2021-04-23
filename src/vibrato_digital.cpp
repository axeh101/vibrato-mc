#include <iostream>
#include "all.hpp"

using namespace std;
int main() {

	double maturity = 1;
	double strike = 100;
	double rate = .05;
	double vol = .2;
	int n = 25;
	int M = 1000;
	int Mz = 10;

	// Product definition
	DigitalOption<double> callDigital(maturity, strike, OptionType::Call);

	// Black Scholes process definition
	State<double> initialState = { 0.0, 100 };
	BlackScholesProcess<double> bs(initialState, rate, vol);

	// Pricing engines definition
	AnalyticalDigitalBS<double> bsEngine = AnalyticalDigitalBS<double>(&callDigital, &bs);
	VibratoBS<double> vibratoEngine = VibratoBS<double>(&callDigital, &bs, n, M,
			Mz);

	// Premium tests
	Path<double> *vibratoPremium = new Path<double>(201);
	Path<double> *analyticPremium = new Path<double>(201);

	// delta tests
	Path<double> *vibratoDelta = new Path<double>(201);
	Path<double> *analyticDelta = new Path<double>(201);
	Path<double> *vibratoDeltaAntithetic = new Path<double>(201);

	// vega tests
	Path<double> *vibratoVega = new Path<double>(201);
	Path<double> *analyticVega = new Path<double>(201);
	Path<double> *vibratoVegaAntithetic = new Path<double>(201);

	// rho tests
	Path<double> *vibratoRho = new Path<double>(201);
	Path<double> *analyticRho = new Path<double>(201);
	Path<double> *vibratoRhoAntithetic = new Path<double>(201);


	double price = 1;
	for (int i = 0; i < 201; ++i) {
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


		price += 1;
	}
	vect2csv("src/python/datasets/digicall_analytic_premium", *analyticPremium);
	vect2csv("src/python/datasets/digicall_vibrato_premium", *vibratoPremium);
	vect2csv("src/python/datasets/digicall_analytic_delta", *analyticDelta);
	vect2csv("src/python/datasets/digicall_analytic_vega", *analyticVega);
	vect2csv("src/python/datasets/digicall_analytic_rho", *analyticRho);

	vect2csv("src/python/datasets/digicall_vibrato_delta", *vibratoDelta);
	vect2csv("src/python/datasets/digicall_vibrato_delta_antithetic",
			*vibratoDeltaAntithetic);
	vect2csv("src/python/datasets/digicall_vibrato_vega", *vibratoVega);
	vect2csv("src/python/datasets/digicall_vibrato_vega_antithetic",
			*vibratoVegaAntithetic);
	vect2csv("src/python/datasets/digicall_vibrato_rho", *vibratoRho);
	vect2csv("src/python/datasets/digicall_vibrato_rho_antithetic",
			*vibratoRhoAntithetic);

//	std::cout << bsEngine << std::endl;
//	std::cout << vibratoEngine << std::endl;

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


	return 0;
}
