#ifndef CORE_UTIL_H
#define CORE_UTIL_H

#include "HybridComputing.h"
#include "Typedef.h"
#define d_abs(myvalue) ((myvalue>0)?(myvalue):-(myvalue))
namespace HyCore
{
    __common double getGrowthRate(double x0, double xn, double dx0, int Npts, double errorTol, int maxIts);
    __common double GetUplusAllmaras(double yPlus);
    __common double MutSA(double turb, double rho, double mu);
    __common void ComputeProductionDestructionSA(double* P, double* D, double rho_in, double mu_in, double turb_in, double y_in, double dudy_in);
    __common void TDMASolve(tdsys& linearSystem, const int num);
    __common double SaBacksolve(double mutIn, double muIn, double rhoIn);
}

#endif
