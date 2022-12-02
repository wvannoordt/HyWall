#ifndef CORE_DAT_H
#define CORE_DAT_H
#include "HybridComputing.h"
#include "UserSettings.h"
#include "EquationTypes.h"
#include "Typedef.h"

namespace HyCore
{
    __common extern int N;
    __common extern double* udns;
    __common extern double* Tdns;
    __common extern double* u;
    __common extern double* u_SA;
    __common extern double* dpdx;
    __common extern double* momBalancedRHS;
    __common extern double* failurelevel;
    __common extern double* x;
    __common extern double* y;
    __common extern double* z;
    __common extern double* d;
    __common extern double* T;
    __common extern double* rho;
    __common extern double* mu;
    __common extern double* turb;
    __common extern double* mu_t;
    __common extern double* lam_t;
    __common extern double* p_F;
    __common extern double* u_F;
    __common extern double* v_F;
    __common extern double* w_F;
    __common extern double* T_F;
    __common extern double* mu_t_F;
    __common extern double* rho_F;
    __common extern double* mu_F;
    __common extern double* distance;
    __common extern double* mom0;
    __common extern double* mom1;
    __common extern double* mom2;
    __common extern double* mom3;
    __common extern double* engy0;
    __common extern double* engy1;
    __common extern double* engy2;
    __common extern double* engy3;
    __common extern double* turb0;
    __common extern double* turb1;
    __common extern double* turb2;
    __common extern double* turb3;
    __common extern double* vorticity;
    __common extern double* tau;
    __common extern double* heatflux;
    __common extern double* error;
    __common extern double* iterations;
    __common extern double* sensorMult;
    __common extern int majorAccessPitch;
    __common extern int momentumEquationClassification;
    __common extern int turbulenceEquationClassification;
    __common extern int energyEquationClassification;
    __common extern HyWall::UserSettings settings;
    __common extern tdsys momSystem;
    __common extern tdsys turbSystem;
    __common extern tdsys engySystem;
    __common extern int solveCount;
}

#endif