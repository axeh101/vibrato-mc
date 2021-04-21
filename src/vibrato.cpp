#include <iostream>
#include "all.hpp"

using namespace std;
int main() {

    double maturity = .5;
    double strike = 100;
	double rate= .02;
	double vol = .03;

	VanillaOption<double> callVanilla(maturity, strike, OptionType::Call);
	VanillaOption<double> putVanilla(maturity, strike, OptionType::Put);
	DigitalOption<double> callDigital(maturity, strike, OptionType::Call);

	State<double> initialState = {0.0, 100};
	BlackScholesProcess<double> bs(initialState, rate, vol);

	AnalyticalBS<double> bsEngineCall = AnalyticalBS<double>(callVanilla, bs);
	AnalyticalBS<double> bsEnginePut = AnalyticalBS<double>(putVanilla, bs);

	VibratoBS<double> vbsEngineCall = VibratoBS<double>(callVanilla, bs, 1000, 1000, 100);
	VibratoBS<double> vbsEnginePut = VibratoBS<double>(putVanilla, bs, 1000, 1000, 100);

	double startingPrice = 90;
	Path<double> deltasCall(201);
	Path<double> deltasPut(201);
	Path<double> deltasVCall(201);
	Path<double> deltasVPut(201);
	double price = startingPrice;

	for (int i = 0; i < 201; ++i) {
		bs.initialState.value = price;
	    bsEngineCall.calculate();
	    bsEnginePut.calculate();
	    vbsEngineCall.calculate();
	    vbsEnginePut.calculate();
		deltasPut[i] = {price, bsEnginePut.delta()};
		deltasCall[i] = {price, bsEngineCall.delta()};
		deltasVPut[i] = {price, vbsEnginePut.delta()};
		deltasVCall[i] = {price, vbsEngineCall.delta()};
		price += 0.1;
	}
    cout << "*****************************" << endl;
	cout << deltasVCall << endl;
    cout << "*****************************" << endl;

	vect2csv("src/python/datasets/bscalldelta", deltasCall);
	vect2csv("src/python/datasets/bsputdelta", deltasPut);
	vect2csv("src/python/datasets/vibratobscalldelta", deltasVCall);
	vect2csv("src/python/datasets/vibratobsputdelta", deltasVPut);
	return 0;
}
