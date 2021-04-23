#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <fstream>
#include "processes/helper.hpp"

using namespace std;

template<typename T>
void vect2csv(std::string name, Path<T> &v) {
	ofstream myfile(name + ".csv");
	if (myfile.is_open()) {
		for (auto i : v) {
			myfile << i << endl;
		}
		myfile.close();
		std::cout << "Writing to file " << name << ".csv....Done! " << endl;
	} else {
		std::cout << "Unable to open file " << name << ".csv" << std::endl;
	}
}

#endif  // UTILS_HPP
