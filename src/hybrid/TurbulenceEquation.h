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
    __common void ComputeAlgebraicTurbulence(const int widx, const int turbEq);
    __common void ZeroTurbInit(const int widx);
    __common void ComputeVanDriestTurbulence(const int widx);
    __common void ComputePNLM(const int widx);
    __common double ComputeYCoord(const int widx, const int i, const int scale_type);
    __common double GetTurbPrandtl(const int widx, const int i, const bool is_variable_prt, const int scale_type);
}

#endif
