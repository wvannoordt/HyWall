#ifndef ALL_EQ_H
#define ALL_EQ_H

#include "HybridComputing.h"
#include "UserSettings.h"
#include "MomentumEquation.h"
#include "TurbulenceEquation.h"
#include "EnergyEquation.h"

namespace HyCore
{
    __common void ComputeLinearSystems(const int widx, const double relaxationFactor);
    __common void SolveUpdateLinearSystems(const int widx, double* errorOut);
    __common void ComputeAlgebraicExpressions(const int widx);
    __common void ComputeExplicitExpressions(const int widx, double* errorOut, double* itsOut);
}

#endif
