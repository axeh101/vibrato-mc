#include <iostream>
#include "all.hpp"

using namespace std;

int main() {

	std::cout << "***** Staring Performance tests !" << std::endl;

	double maturity = 1;
	double strike = 100;
	double rate = .05;
	double vol = .2;
	int n = 25;
<<<<<<< HEAD
	int M = 1;
	int Mz = 100;
=======
	int M = 10000;
	int Mz = 1;
>>>>>>> 064b01b426d6351ccf3163ea2b0f33c6f939a510

	// Product definition
	VanillaOption<double> callDigital(maturity, strike, OptionType::Call);

	// Black Scholes process definition
	State<double> initialState = { 0.0, 100 };
	BlackScholesProcess<double> bs(initialState, rate, vol);

	// Pricing engines definition
	AnalyticalVanillaBS<double> bsEngine = AnalyticalVanillaBS<double>(
			&callDigital, &bs);
	VibratoBS<double> vibratoEngine = VibratoBS<double>(&callDigital, &bs, n, M,
			Mz);

	// Convergence  tests (Vibrato vs Vibrato antithetic)
<<<<<<< HEAD
	Path<double> *analyticValue = new Path<double>(1001);
	Path<double> *vibratoConvergence = new Path<double>(1001);
	Path<double> *vibratoAntitheticConvergence = new Path<double>(1001);
	bsEngine.calculate();
	double delta = bsEngine.delta();
	for (int i = 0; i < 1001; ++i) {
=======
	Path<double> *analyticValue = new Path<double>(101);
	Path<double> *vibratoConvergence = new Path<double>(101);
	Path<double> *vibratoAntitheticConvergence = new Path<double>(101);
	bsEngine.calculate();
	double delta = bsEngine.delta();
	for (int i = 0; i < 101; ++i) {
>>>>>>> 064b01b426d6351ccf3163ea2b0f33c6f939a510
		vibratoEngine.M += 100;
		vibratoEngine.antithetic = false;
		vibratoEngine.calculate();
		(*analyticValue)[i] = {(double)vibratoEngine.M , delta};
		(*vibratoConvergence)[i] = {(double)vibratoEngine.M, vibratoEngine.delta()};

		vibratoEngine.antithetic = true;
		vibratoEngine.calculate();
		(*vibratoAntitheticConvergence)[i] = {(double)vibratoEngine.M , vibratoEngine.delta()};
<<<<<<< HEAD
=======
		std::cout << vibratoEngine.M << std::endl;
>>>>>>> 064b01b426d6351ccf3163ea2b0f33c6f939a510
	}
	vect2csv("src/python/datasets/perf_delta_analytic", *analyticValue);
	vect2csv("src/python/datasets/perf_delta_vibrato", *vibratoConvergence);
	vect2csv("src/python/datasets/perf_delta_vibrato_anti", *vibratoAntitheticConvergence);



	delete analyticValue;
	delete vibratoConvergence;
	delete vibratoAntitheticConvergence;


	std::cout << "***** Performance tests terminated!" << std::endl;

	return 0;
}