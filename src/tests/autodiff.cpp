//
// Created by abenmoussa on 4/29/21.
//
#include <iostream>
#include <autodiff/forward.hpp>

using namespace autodiff;


dual f(dual x) {
    dual val = x - 5;
    return val > 0 ? val : 0;
}

int main() {
    dual x = 0;
    dual y = 0;
    for (int i = 0; i < 10; i++) {
        y += 1;
        std::cout << i << ": y = " << y << std::endl;
        std::cout << i << ": f(y) = " << f(y) << std::endl;
        std::cout << i << ": df/dy = " << derivative(f, wrt(y), at(y)) << std::endl;
    }
    return 0;
}