#ifndef TURB_EQ_H
#define TURB_EQ_H

#include "HybridComputing.h"
#include "UserSettings.h"

namespace HyCore
{
    __common void SolveUpdateSystemTurbulence(const int widx, double* errorOut);
    __common void ComputeLhsRhsTurbulenceODE(const int widx, const double relaxationFactor);
    __common void ComputeLhsRhsTurbulence(const int widx, const double relaxationFactor);
    __common void ComputeExplicitTurbulenceEquation(const int widx, const int turbEq, double* errorOut, double* itsOut);
    __common void InitializeTurbulence(const int widx);
    __common void LinearTurbInit(const int widx);
    __common bool TurbulenceHasJacobian(HyWall::UserSettings* inputSettings);
}

#endif
