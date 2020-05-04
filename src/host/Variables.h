#ifndef VARS_H
#define VARS_H

#include "HyWall.h"

namespace HyWall
{
    void DefineInputVariables(void);
    void DefineAuxilaryVariables(void);
    void DefineMomentumVariables(void);
    void DefineEnergyVariables(void);
    void DefineTurbulentVariables(void);
    void DefineOutputVariables(void);
    bool HasFlag(int a, int flag);
}

#endif
