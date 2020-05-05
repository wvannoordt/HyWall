#include "HybridComputing.h"
#include "DebugTools.h"
#include <iostream>
#include "CoreData.h"
#include "CoreUtils.h"
#include "Constants.h"
#if(__cpu)
#include <cmath>
using std::pow;
using std::atan2;
using std::log;
#endif

namespace HyCore
{
    __common double getGrowthRate(double x0, double xn, double dx0, int Npts, double errorTol, int maxIts)
    {
        double Z = (xn - x0) / dx0;
        double epsilon = 100;
        double guess = 2.0;
        int k = 0;
        while (d_abs(epsilon) > errorTol && k++ < maxIts)
        {
            epsilon = pow(guess,(Npts - 1)) - Z*(guess) + Z - 1.0;
            guess = guess - (epsilon) / ((Npts-1)*pow(guess, (Npts - 2)) - Z);
        }
        if ((guess < 1.0) || (d_abs(epsilon) > errorTol)) __erkill("Could not generate grid for x0=" << x0 << ", xn=" << xn << ", dx0=" << dx0 << ". Try adjusting wall spacing.");        
        return guess;
    }

    __common double GetUplusAllmaras(double yPlus)
    {
        return CONST_ALGEBRAIC_B
				+CONST_ALGEBRAIC_C1*log((yPlus+CONST_ALGEBRAIC_A1)*(yPlus+CONST_ALGEBRAIC_A1)+CONST_ALGEBRAIC_B1*CONST_ALGEBRAIC_B1)
		        -CONST_ALGEBRAIC_C2*log((yPlus+CONST_ALGEBRAIC_A2)*(yPlus+CONST_ALGEBRAIC_A2)+CONST_ALGEBRAIC_B2*CONST_ALGEBRAIC_B2)
	            -CONST_ALGEBRAIC_C3*atan2(CONST_ALGEBRAIC_B1, yPlus+CONST_ALGEBRAIC_A1)-CONST_ALGEBRAIC_C4*atan2(CONST_ALGEBRAIC_B2, yPlus+CONST_ALGEBRAIC_A2);
    }
}
