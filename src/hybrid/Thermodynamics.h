#ifndef THERM_H
#define THERM_H

#include "HybridComputing.h"

namespace HyCore
{
    __common double SutherlandViscosityRelation(double T);
    __common double DensityRelation(double P, double T);
    __common void EquationsOfState(const int widx);
}

#endif
