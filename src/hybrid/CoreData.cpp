#include "HybridComputing.h"
#include "UserSettings.h"
#include "CoreData.h"
#include "Typedef.h"

namespace HyCore
{
    __common int N;
    __common double* u;
    __common double* d;
    __common double* T;
    __common double* rho;
    __common double* mu;
    __common double* turb;
    __common double* p_F;
    __common double* u_F;
    __common double* v_F;
    __common double* w_F;
    __common double* T_F;
    __common double* turb_F;
    __common double* distance;
    __common double* mom0;
    __common double* mom1;
    __common double* mom2;
    __common double* mom3;
    __common double* engy0;
    __common double* engy1;
    __common double* engy2;
    __common double* engy3;
    __common double* turb0;
    __common double* turb1;
    __common double* turb2;
    __common double* turb3;
    __common double* vorticity;
    __common double* tau;
    __common double* heatflux;
    __common double* error;
    __common double* iterations;
    __common int majorAccessPitch;
    __common int momentumEquationClassification;
    __common int turbulenceEquationClassification;
    __common int energyEquationClassification;
    __common HyWall::UserSettings settings;
    __common tdsys momSystem;
    __common tdsys turbSystem;
    __common tdsys engySystem;
}
