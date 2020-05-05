#ifndef CORE_UTIL_H
#define CORE_UTIL_H

#include "HybridComputing.h"
#include "DebugTools.h"
#include <iostream>
#include "CoreData.h"
#if(__cpu)
#include <cmath>
using std::pow;
#endif

namespace HyCore
{
    __common double getGrowthRate(double x0, double xn, double dx0, int Npts, double errorTol, int maxIts)
    {
        double Z = (xn - x0) / dx0;
        double epsilon = 100;
        double guess = 2.0;
        int k = 0;
        while (abs(epsilon) > errorTol && k++ < maxIts)
        {
            epsilon = pow(guess,(N - 1)) - Z*(guess) + Z - 1.0;
            guess = guess - (epsilon) / ((N-1)*pow(guess, (N - 2)) - Z);
        }
        if ((guess < 1.0) || (abs(epsilon) > errorTol)) __erkill("Could not generate grid for x0=" << x0 << ", xn=" << xn << ", dx0=" << dx0 << ". Try adjusting wall spacing.");
        return guess;
    }
}

#endif
