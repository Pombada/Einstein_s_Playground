//
// Created by Dumbledore on 3/1/2026.
//

#ifndef EINSTEIN_S_PLAYGROUND_BUTTONS_H
#define EINSTEIN_S_PLAYGROUND_BUTTONS_H
#include <string>
class RelativityCalculator {
public:
    static constexpr double C = 299792458.0; // Speed of light in m/s
    static long double CalculateGamma(long double v);
    static double ParseAndCalculate(std::wstring input);

};
#endif //EINSTEIN_S_PLAYGROUND_BUTTONS_H