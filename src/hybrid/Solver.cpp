#include "HybridComputing.h"
#include "Solver.h"
#include "DebugTools.h"
#include <iostream>
#include <fstream>
#include "Indexing.h"
#include "CoreData.h"
#include "AllEquations.h"
#include "CoreUtils.h"
#if(__cpu)
#include <cmath>
using std::sqrt;
#endif
namespace HyCore
{
    __common void MetaDataSet(HyWall::UserSettings* inputSettings)
    {
        SetMomentumEquationType(inputSettings);
        SetEnergyEquationType(inputSettings);
        SetTurbulenceEquationType(inputSettings);
        N = inputSettings->rayDim;
    }

    __common void BuildGrid(const int widx)
    {
        double growthRate = getGrowthRate(0, elem(distance, widx), settings.wallSpacing, N, settings.errorTolerance, settings.maxIterations);
        elem(d, widx, 0) = 0.0;
        elem(d, widx, 1) = settings.wallSpacing;
        elem(d, widx, N-1) = elem(distance, widx);
        for (int i = 2; i < N-1; i++)
        {
            elem(d, widx, i) = elem(d, widx, i-1) + growthRate*(elem(d, widx, i-1)-elem(d, widx, i-2));
        }
    }

    __common void Initialize(const int widx)
    {
        BuildGrid(widx);
        UpdateBoundaryConditions(widx);
        InitializeEnergy(widx);
        InitializeTurbulence(widx);
        InitializeMomentum(widx);
    }

    __common void UpdateBoundaryConditions(const int widx)
    {
        elem(u,    widx, N-1) = elem(u_F,    widx);
        elem(T,    widx, N-1) = elem(T_F,    widx);
        elem(mu_t, widx, N-1) = elem(mu_t_F, widx);
        elem(turb, widx, N-1) = SaBacksolve(elem(mu_t_F, widx), elem(mu_F, widx), elem(rho_F, widx));
        elem(rho,  widx, N-1) = elem(rho_F,  widx);
        elem(mu,   widx, N-1) = elem(mu_F,   widx);
    }

    __common void EquationsOfState(const int widx)
    {
        if (settings.turbulenceEquationType == turbulence::ODE) for (int i = 0; i < N; i++) elem(mu_t, widx, i) = MutSA(elem(turb, widx, i), elem(rho, widx, i), elem(mu, widx, i));
    }

    __common void MainSolver(const int widx)
    {
        bool done = false;
        double totalError = 0.0;
        double totalIts = 0.0;
        double localError = 0.0;
        double localIts = 0.0;
        int numIts = 0;
        double relaxationFactor = 0.2;
        double growth = 1.1;
        UpdateBoundaryConditions(widx);
        EquationsOfState(widx);
        ComputeExplicitExpressions(widx, &localError, &localIts);
        totalError += localError;
        totalIts += localIts;
        localError = 100000;
        localIts = 0.0;

        while ((d_abs(localError) > settings.errorTolerance) && (numIts < settings.maxIterations))
        {
            ComputeLinearSystems(widx, relaxationFactor);
            SolveUpdateLinearSystems(widx, &localError);
            EquationsOfState(widx);
            ComputeAlgebraicExpressions(widx);
            localIts+=1.0;
            relaxationFactor *= growth;
            relaxationFactor = (relaxationFactor>1.0)?1.0:relaxationFactor;
            numIts++;
        }
        totalError += localError;
        totalIts += localIts;
        if (d_abs(localError) > settings.errorTolerance)
        {
            double umag = sqrt(elem(u_F,widx)*elem(u_F,widx) + elem(v_F,widx)*elem(v_F,widx) + elem(w_F,widx)*elem(w_F,widx));
            __erkill("Failed wall model solve at widx=" << widx << ": |u_F|=" << umag << ", error=" << localError << ", " << "numIts=" << numIts);
        }

        double u1  = elem(u, widx, 1);
        double mu1 = elem(mu, widx, 1);
        elem(error, widx) = totalError;
        elem(iterations, widx) = totalIts;
        elem(tau, widx) = mu1*u1/settings.wallSpacing;
        elem(vorticity, widx) = u1/settings.wallSpacing;
        elem(heatflux, widx) = -(settings.fluidCp*mu1/settings.fluidPrandtl)*(elem(T, widx, 1)-elem(T, widx, 0))/settings.wallSpacing;

    }

    __common void SetMomentumEquationType(HyWall::UserSettings* inputSettings)
    {
        switch(inputSettings->momentumEquationType)
        {
            case momentum::allmaras:
            {
                momentumEquationClassification = equationType::explicitEq;
                break;
            }
            case momentum::ODE:
            {
                momentumEquationClassification = equationType::differentialEq;
                break;
            }
        }
    }

    __common void SetEnergyEquationType(HyWall::UserSettings* inputSettings)
    {
        switch(inputSettings->energyEquationType)
        {
            case energy::croccoBusemann:
            {
                energyEquationClassification = equationType::algebraicEq;
                break;
            }
            case energy::linear:
            {
                energyEquationClassification = equationType::explicitEq;
                break;
            }
            case energy::ODE:
            {
                energyEquationClassification = equationType::differentialEq;
                break;
            }
        }
    }

    __common void SetTurbulenceEquationType(HyWall::UserSettings* inputSettings)
    {
        switch(inputSettings->turbulenceEquationType)
        {
            case turbulence::linear:
            {
                turbulenceEquationClassification = equationType::explicitEq;
                break;
            }
            case turbulence::ODE:
            {
                turbulenceEquationClassification = equationType::differentialEq;
                break;
            }
            case turbulence::vanDriest:
            {
                turbulenceEquationClassification = equationType::algebraicEq;
                break;
            }
        }
    }
}
