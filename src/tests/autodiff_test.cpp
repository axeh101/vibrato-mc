#include <iostream>

using namespace std;

#include <cmath>
#include <autodiff/forward.hpp>

using namespace autodiff;
using namespace std;

dual f(dual x, dual y, dual z) {
    double k = 0;
    return k * exp(-x * x) + max(x, y) + z;
}

dual g(dual x, dual y, dual z) {
    return x * f(x, y, z);
}

int main() {
    dual x = 2.0;
    dual y = 5;
    dual z = 1.;
    dual u = g(x, y, z);

    double dudx = derivative(g, wrt(y), at(x, y, z));
    cout << "u = " << u << endl;
    cout << "du/dx = " << dudx << endl;

}
