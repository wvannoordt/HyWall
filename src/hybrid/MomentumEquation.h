#ifndef MOM_EQ_H
#define MOM_EQ_H

#include "HybridComputing.h"
#include "UserSettings.h"

namespace HyCore
{
    __common void ComputeAllmarasMomentum(const int widx, double* errorOut, double* itsOut);
    __common double NewtonIterationAllmaras(const int widx, double* errorOut, double* itsOut);
    __common void ComputeLhsRhsMomentumODE(const int widx);
    __common void SolveUpdateSystemMomentum(const int widx, double* errorOut);
    __common void ComputeLhsRhsMomentum(const int widx, const double relaxationFactor);
    __common void ComputeExplicitMomentumEquation(const int widx, const int momEq, double* errorOut, double* itsOut);
    __common void InitializeMomentum(const int widx);
    __common void LinearUInit(const int widx);
    __common bool MomentumHasJacobian(HyWall::UserSettings* inputSettings);
}

#endif
