#ifndef SOLVER_H
#define SOLVER_H

#include "HybridComputing.h"

namespace HyCore
{
    __common void MetaDataSet(void);
    __common void Initialize(const int idx);
    __common void MainSolver(const int idx);
    __common void SetMomentumEquationType(void);
    __common void SetEnergyEquationType(void);
    __common void SetTurbulenceEquationType(void);
}

#endif
