#ifndef VIBRATO_MC_HELPER_H
#define VIBRATO_MC_HELPER_H

#include "../all.hpp"

using namespace std;


class Helper {
public:
    static void generateGreek(Option<double> *o,
                              Process<double> *p,
                              std::function<double()> greek,
                              double price,
                              size_t size,
                              int step,
                              std::string filename) {
        auto *vec = new Path<double>(size);
        for (size_t i = 0; i < size; ++i) {
            p->initialState.value = price;
            (*vec)[i] = {price, greek()};
            price += step;


        };
        std::string destination = "src/python/datasets/";
        std::string type = o->type() == 1 ? "call" : "put";
        vect2csv(destination + type + filename, *vec);
        delete vec;
    }
};


#endif //VIBRATO_MC_HELPER_H
