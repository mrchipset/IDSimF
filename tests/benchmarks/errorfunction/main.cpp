#include "CollisionModel_MathFunctions.hpp"
#include <iostream>
#include <algorithm>
#include <vector>
#include <ctime>
#include <cmath>

double generateFct()
{
    static double v = 1;
    v = v + 0.01;
    return v;
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Benchmark errorfunction" << std::endl;
    clock_t begin = std::clock();

    int nSamples = 80000000;
    std::vector<double> xVec(nSamples);
    std::generate(xVec.begin(), xVec.end(), generateFct);
    std::vector<double> result_std(nSamples);

    for (int i=0; i < nSamples; ++i){
        result_std[i] = std::erf(xVec[i]);
    }

    clock_t end = std::clock();
    double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

    std::cout << "elapsed secs: "<< elapsed_secs<<std::endl;
    std::cout << "result[0]: " << result_std[0] << std::endl;
    return 0;
}
