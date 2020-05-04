#ifndef CORE_DAT_H
#define CORE_DAT_H
#include "HybridComputing.h"
#include "UserSettings.h"

namespace HyCore
{
    __common extern double* u;
    __common extern double* d;
    __common extern double* T;
    __common extern double* rho;
    __common extern double* mu;
    __common extern double* turb;
    __common extern double* p_F;
    __common extern double* u_F;
    __common extern double* v_F;
    __common extern double* w_F;
    __common extern double* T_F;
    __common extern double* turb_F;
    __common extern double* vorticity;
    __common extern double* tau;
    __common extern double* heatflux;
    __common extern double* error;
    __common extern double* iterations;
    __common extern HyWall::UserSettings settings;
}

#endif
