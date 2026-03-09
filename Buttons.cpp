//
// Created by Dumbledore on 3/1/2026.
//
#include "Buttons.h"
#include <cmath>
#include <iostream>
#include <ostream>
#include <stdexcept>
#define C 299792458.0L
long double RelativityCalculator::CalculateGamma(long double v) {
    if (v < 0 || v >= C) return -1.0;
    return 1.0L / std::sqrt(1.0L - (v * v) / (C * C));
}

long double RelativityCalculator::ParseAndCalculate(std::wstring input) {
    // 1. Convert to lowercase for easier comparison
    for (auto &c : input) c = towlower(c);

    double multiplier = 1.0;
    double baseValue = 0.0;
    if (input.length() > 1 && input[0] == L'0' && iswdigit(input[1])) {
        return -2.0L;
    }
    try {
        // 2. Check for Percentage
        size_t pctPos = input.find(L"%");
        if (pctPos != std::wstring::npos) {
            double percent = std::stod(input.substr(0, pctPos));
            return CalculateGamma(C * (percent / 100.0));
        }

        // 3. Check for suffixes (k or m)
        if (input.find(L"m") != std::wstring::npos) {
            multiplier = 1000000.0;
            input.erase(input.find(L"m")); // Remove 'm' for stod
        } else if (input.find(L"k") != std::wstring::npos) {
            multiplier = 1000.0;
            input.erase(input.find(L"k")); // Remove 'k' for stod
        }

        baseValue = std::stod(input);
       long double v = baseValue * multiplier;

        return CalculateGamma(v);

    } catch (...) {
        return -2.0; // Error: Invalid Input
    }
}

long double RelativityCalculator::CalculateSpeedFromGamma(long double gamma) {
    if (gamma <= 1.0L) return 0.0L; // Gamma cannot be less than 1
    // v = c * sqrt(1 - 1/gamma^2)
    return C * std::sqrt(1.0L - (1.0L / (gamma * gamma)));
}
long double RelativityCalculator::ParseGammaInput(std::wstring input) {
    try {
        if (input.empty()) return -2.0L;
        if (input.length() > 1 && input[0] == L'0' && iswdigit(input[1])) {
            return -2.0L;
        }
        // Validate: only digits and one decimal point allowed
        int decimalCount = 0;
        for (auto &c : input) {
            if (c == L'.') {
                decimalCount++;
                if (decimalCount > 1) throw std::invalid_argument("Too many decimals");
            } else if (!iswdigit(c)) {
                throw std::invalid_argument("Invalid character");
            }
        }

        long double g = std::stold(input);

        if (g < 1.0L) return -1.0L; // Gamma must be >= 1

        return CalculateSpeedFromGamma(g);

    } catch (...) {
        return -2.0L; // General error code for "Invalid Input"
    }
}
