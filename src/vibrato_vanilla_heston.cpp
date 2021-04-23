#include <iostream>
#include "all.hpp"

using namespace std;

int main() {

	std::cout << "***** Staring vibrato for options (Heston) !" << std::endl;

	double maturity = 1;
	double strike = 100;
	double rate = .05;
	int n = 100;
	int M = 1000;
	int Mz = 10;
	double vol = .02;
	// Heston params
	double initialVol = .6;
	double correl = -0.6;
	double kappa = 16;
	double eta = 0.1;
	double volvol = 0.3;

	// Product definition
	VanillaOption<double> call(maturity, strike, OptionType::Call);

	// Heston & CIR process definition
	State<double> initialState = { 0.0, 100 };
	CoxIngersollRossProcess<double> cir( { 0, initialVol}, kappa, eta, volvol);
	HestonProcess<double> heston(initialState, &cir, rate, correl);
	BlackScholesProcess<double> bs(initialState, rate, vol);

	// Pricing engines definition
	VibratoBS<double> ve = VibratoBS<double>(&call, &bs, n, M, Mz);
	AnalyticalDigitalBS<double> be = AnalyticalDigitalBS<double>(&call, &bs);
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

	double price = 60;
	for (int i = 0; i < 201; ++i) {
		bs.initialState.value = price;
		be.calculate();
		(*analyticPremium)[i] = { price, be.premium() };
		(*analyticDelta)[i] = { price, be.delta() };
		(*analyticVega)[i] = { price, be.vega() };
		(*analyticRho)[i] = { price, be.rho() };

		ve.antithetic = true;
		ve.calculate();
		(*vibratoPremium)[i] = { price, ve.premium() };
		(*vibratoVegaAntithetic)[i] = { price, ve.vega() };
		(*vibratoDeltaAntithetic)[i] = { price, ve.delta() };
		(*vibratoRhoAntithetic)[i] = { price, ve.rho() };

		ve.antithetic = false;
		ve.calculate();
		(*vibratoDelta)[i] = { price, ve.delta() };
		(*vibratoVega)[i] = { price, ve.vega() };
		(*vibratoRho)[i] = { price, ve.rho() };

		price += 0.5;
	}
	vect2csv("src/python/datasets/call_heston_analytic_premium", *analyticPremium);
	vect2csv("src/python/datasets/call_heston_analytic_delta", *analyticDelta);
	vect2csv("src/python/datasets/call_heston_analytic_vega", *analyticVega);
	vect2csv("src/python/datasets/call_heston_analytic_rho", *analyticRho);

	vect2csv("src/python/datasets/call_heston_vibrato_premium", *vibratoPremium);

	vect2csv("src/python/datasets/call_heston_vibrato_delta", *vibratoDelta);
	vect2csv("src/python/datasets/call_heston_vibrato_delta_antithetic",
			*vibratoDeltaAntithetic);
	vect2csv("src/python/datasets/call_heston_vibrato_vega", *vibratoVega);
	vect2csv("src/python/datasets/call_heston_vibrato_vega_antithetic",
			*vibratoVegaAntithetic);
	vect2csv("src/python/datasets/call_heston_vibrato_rho", *vibratoRho);
	vect2csv("src/python/datasets/call_heston_vibrato_rho_antithetic",
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

	std::cout << "***** Vibrato for vanilla (Heston) terminated!" << std::endl;

	return 0;
}
