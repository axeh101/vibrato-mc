#include <iostream>
#include "../all.hpp"

using namespace std;

int main() {

    std::cout << "***** Staring speed tests !" << std::endl;

    const std::string destination = "src/python/datasets/";

    dual maturity = 1;
    dual strike = 100;
    dual price = 50;
    dual rate = .05;
    dual vol = .2;
    int n = 25;
    int Mz = 1;
    int M = 1;

    // Product definition
    VanillaOption<dual> call(maturity, strike, OptionType::Call);

    // Black Scholes process definition
    State<dual> initialState = {0.0, price};
    BlackScholesProcess<dual> bs(initialState, rate, vol);

    // Pricing engines definition
    auto vibratoEngine = Vibrato<dual>(&call, &bs, n, M, Mz);
    auto vadEngine = VibratoAD<dual>(&call, &bs, n, M, Mz);


    int vecSize = 100;
    auto *vSpeed = new Path<dual>(vecSize);
    auto *vadSpeed = new Path<dual>(vecSize);

    auto *vSpeedAnti = new Path<dual>(vecSize);
    auto *vadSpeedAnti = new Path<dual>(vecSize);
    std::chrono::time_point<std::chrono::system_clock> start;
    std::chrono::time_point<std::chrono::system_clock> end;
    std::chrono::duration<double> diff1, diff2, diff3, diff4;
    int j = 0;
    for (int i = 1000; i <= 100000; i += 1000) {
        vibratoEngine.M = i;
        vadEngine.M = i;
        for (int k = 0; k < 100; k++) {
            vibratoEngine.antithetic = true;
            vadEngine.antithetic = true;
            start = std::chrono::system_clock::now();
            vibratoEngine.gamma();
            end = std::chrono::system_clock::now();
            diff1 +=  + (end - start);

            start = std::chrono::system_clock::now();
            vadEngine.gamma();
            end = std::chrono::system_clock::now();
            diff2 += (end - start);

            vibratoEngine.antithetic = false;
            vadEngine.antithetic = false;
            start = std::chrono::system_clock::now();
            vibratoEngine.gamma();

            end = std::chrono::system_clock::now();
            diff3 += (end - start);

            start = std::chrono::system_clock::now();
            vadEngine.gamma();
            end = std::chrono::system_clock::now();
            diff4 += (end - start);

        }
        (*vSpeedAnti)[j] = { i, diff1.count() / 100};
        (*vadSpeedAnti)[j] = { i, diff2.count() / 100};
        (*vSpeed)[j] = {(dual) i, diff3.count() / 100};
        (*vadSpeed)[j++] = { i, diff4.count() / 100};

        cout << "Done for i = " << i << endl;
    }


    vect2csv(destination + "speed_gamma_vad", *vadSpeed);
    vect2csv(destination + "speed_gamma_vad_anti", *vadSpeedAnti);
    vect2csv(destination + "speed_gamma_vibrato", *vSpeed);
    vect2csv(destination + "speed_gamma_vibrato_anti", *vSpeedAnti);

    delete vSpeed;
    delete vSpeedAnti;
    delete vadSpeedAnti;
    delete vadSpeed;

    std::cout << "***** Speed tests terminated!" << std::endl;
    return 0;
}