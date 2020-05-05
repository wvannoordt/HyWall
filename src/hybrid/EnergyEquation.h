#ifndef ENGY_EQ_H
#define ENGY_EQ_H
#include "HybridComputing.h"
#include "UserSettings.h"

namespace HyCore
{
    __common void SolveUpdateSystemEnergy(const int widx, double* errorOut);
    __common void ComputeLhsRhsEnergyODE(const int widx);
    __common void ComputeLhsRhsEnergy(const int widx);
    __common void ComputeExplicitEnergyEquation(const int widx, const int engyEq, double* errorOut, double* itsOut);
    __common void InitializeEnergy(const int widx);
    __common void LinearTInit(const int widx);
    __common bool EnergyHasJacobian(HyWall::UserSettings* inputSettings);
}

#endif
