//
// Created by Dumbledore on 3/1/2026.
//
#include "Buttons.h"
#include <cmath>
long double RelativityCalculator::CalculateGamma(long double v) {
    const long double c = 299792458.0L; // Note the 'L' for long double
    if (v < 0 || v >= c) return -1.0;
    return 1.0L / std::sqrt(1.0L - (v * v) / (c * c));
}

double RelativityCalculator::ParseAndCalculate(std::wstring input) {
    const double c = 299792458.0;

    // 1. Convert to lowercase for easier comparison
    for (auto &c : input) c = towlower(c);

    double multiplier = 1.0;
    double baseValue = 0.0;

    try {
        // 2. Check for Percentage
        size_t pctPos = input.find(L"%");
        if (pctPos != std::wstring::npos) {
            double percent = std::stod(input.substr(0, pctPos));
            return CalculateGamma(c * (percent / 100.0));
        }

        // 3. Check for suffixes (k or m)
        if (input.find(L"m") != std::wstring::npos) {
            multiplier = 1000000.0;
            input.erase(input.find(L"m")); // Remove 'm' for stod
        } else if (input.find(L"k") != std::wstring::npos) {
            multiplier = 1000.0;
            input.erase(input.find(L"k")); // Remove 'k' for stod
        }

        // 4. Convert remaining string to double and apply multiplier
        baseValue = std::stod(input);
        double v = baseValue * multiplier;

        return CalculateGamma(v);

    } catch (...) {
        return -2.0; // Error: Invalid Input
    }
}
