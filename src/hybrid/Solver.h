#ifndef SOLVER_H
#define SOLVER_H

#include "HybridComputing.h"
#include "UserSettings.h"

namespace HyCore
{
    __common void MetaDataSet(HyWall::UserSettings* inputSettings);
    __common void Initialize(const int idx);
    __common void MainSolver(const int idx);
    __common void SetMomentumEquationType(HyWall::UserSettings* inputSettings);
    __common void SetEnergyEquationType(HyWall::UserSettings* inputSettings);
    __common void SetTurbulenceEquationType(HyWall::UserSettings* inputSettings);
    __common void UpdateBoundaryConditions(const int widx);
    __common void BuildGrid(const int widx);
}

#endif
