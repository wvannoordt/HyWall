#ifndef ALL_EQ_H
#define ALL_EQ_H

#include "HybridComputing.h"

namespace HyCore
{
    __common void InitializeTurbulence(const int widx);
    __common void InitializeMomentum(const int widx);
    __common void InitializeEnergy(const int widx);
    __common void ComputeExplicitExpressions(const int widx);
    __common void ComputeExplicitMomentumEquation(const int widx, const int momEq);
    __common void ComputeExplicitTurbulenceEquation(const int widx, const int turbEq);
    __common void ComputeExplicitEnergyEquation(const int widx, const int engyEq);
}

#endif
