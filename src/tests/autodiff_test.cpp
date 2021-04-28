#include <iostream>

using namespace std;

#include <cmath>
#include <autodiff/forward.hpp>

using namespace autodiff;
using namespace std;

dual f(dual x, dual y) {
    return ((x-y) >= (dual)0)?(x-y):(dual)0  ;
}


int main() {
    dual x = 6;
    dual y = 6;
    dual u = f(x, y);

    double dudx = derivative(f, wrt(y), at(x, y));
    cout << "u = " << u << endl;
    cout << "du/dx = " << dudx << endl;

}
