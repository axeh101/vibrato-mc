#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

inline unsigned p2(unsigned n) { unsigned u=1; return u<<=n;}


template<typename T>
void vect2csv(std::string name, std::vector<T>& v){
    std::cout << " Writing to file " << endl;
    ofstream myfile (name + ".csv");
    if (myfile.is_open())
    {
        for (auto i:v)
        {
                        
            myfile << i << endl;
        }
        myfile.close();
    std::cout << " Writing to file.....Done! " << endl;
    }
    else
    {
        std::cout << "Unable to open file" << std::endl;
    }
}


#endif  // UTILS_HPP
