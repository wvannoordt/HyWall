#ifndef CORE_UTIL_H
#define CORE_UTIL_H

#include "HybridComputing.h"

namespace HyCore
{
    __common double getGrowthRate(double x0, double xn, double dx0, int Npts, double errorTol, int maxIts);
}

#endif
