#include <iostream>
#include "../all.hpp"

using namespace std;

int main() {

    std::cout << "***** Staring speed tests !" << std::endl;

    const std::string destination = "src/python/datasets/";

    double maturity = 1;
    double strike = 100;
    double rate = .05;
    double vol = .2;
    int n = 25;
    int Mz = 1;
    int M = 1;

    // Product definition
    VanillaOption<double> call(maturity, strike, OptionType::Call);

    // Black Scholes process definition
    State<double> initialState = {0.0, 80};
    BlackScholesProcess<double> bs(initialState, rate, vol);

    // Pricing engines definition
    auto vibratoEngine = Vibrato<double>(&call, &bs, n, M, Mz);
    auto vadEngine = VibratoAD<double>(&call, &bs, n, M, Mz);


    int vecSize = 50000 - 10;
    auto *vSpeed = new Path<double>(vecSize);
    auto *vadSpeed = new Path<double>(vecSize);

    auto *vSpeedAnti = new Path<double>(vecSize);
    auto *vadSpeedAnti = new Path<double>(vecSize);
    std::chrono::time_point<std::chrono::system_clock> start;
    std::chrono::time_point<std::chrono::system_clock> end;
    std::chrono::duration<double> diff;
    for (int i = 10; i < 50001; ++i) {

        vibratoEngine.M = i;
        vadEngine.M = i;

        start = std::chrono::system_clock::now();
        vibratoEngine.gamma();
        end = std::chrono::system_clock::now();
        diff = end - start;
        (*vSpeedAnti)[i - 1] = {(double) i, diff.count()};

        start = std::chrono::system_clock::now();
        vadEngine.gamma();
        end = std::chrono::system_clock::now();
        diff = end - start;
        (*vadSpeedAnti)[i - 1] = {(double) i, diff.count()};

        vibratoEngine.antithetic = false;
        vadEngine.antithetic = false;

        start = std::chrono::system_clock::now();
        vibratoEngine.gamma();
        end = std::chrono::system_clock::now();
        diff = end - start;
        (*vSpeed)[i - 1] = {(double) i, diff.count()};

        start = std::chrono::system_clock::now();
        vadEngine.gamma();
        end = std::chrono::system_clock::now();
        diff = end - start;
        (*vadSpeed)[i - 1] = {(double) i, diff.count()};

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