//
// Created by Dumbledore on 3/1/2026.
//
#include "Buttons.h"
#include <cmath>
double RelativityCalculator::CalculateGamma(double velocity) {
    if (velocity >= C) return -1.0; // Error: faster than light
    if (velocity < 0) velocity = -velocity; // Velocity is scalar here
    double beta = velocity / C;
    return 1.0 / sqrt(1.0 - (beta * beta));
}
