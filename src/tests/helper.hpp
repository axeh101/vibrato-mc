#ifndef VIBRATO_MC_HELPER_H
#define VIBRATO_MC_HELPER_H

#include <functional>
#include "../all.hpp"

using namespace std;

template<typename T>
class Helper {
public:
    static void generateGreek(Option<T> *o,
                              Process<T> *p,
                              std::function<T()> greek,
                              T price,
                              size_t size,
                              int step,
                              std::string filename) {
        auto *vec = new Path<T>(size);
        p->resetState();
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
