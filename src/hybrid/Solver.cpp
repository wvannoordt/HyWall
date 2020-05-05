#include "HybridComputing.h"
#include "Solver.h"
#include "DebugTools.h"
#include <iostream>
#include "Indexing.h"
#include "CoreData.h"
#include "AllEquations.h"
#include "CoreUtils.h"
namespace HyCore
{
    __common void MetaDataSet(void)
    {
        SetMomentumEquationType();
        SetEnergyEquationType();
        SetTurbulenceEquationType();
        N = settings.rayDim;
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
        InitializeTurbulence(widx);
        InitializeMomentum(widx);
        InitializeEnergy(widx);
    }

    __common void UpdateBoundaryConditions(const int widx)
    {
        elem(u,    widx, N-1) = elem(u_F,    widx);
        elem(T,    widx, N-1) = elem(T_F,    widx);
        elem(turb, widx, N-1) = elem(turb_F, widx);
    }

    __common void MainSolver(const int widx)
    {
        bool done = false;
        bool failed = false;
        double totalError = 0.0;
        int numIts = 0;
        UpdateBoundaryConditions(widx);
        ComputeExplicitExpressions(widx);
        while (!done)
        {

            done = (abs(totalError) < settings.errorTolerance) || (numIts++ < settings.maxIterations);
            failed = abs(totalError) > settings.errorTolerance;
        }

        if (failed) __erkill("Failed wall model solve at widx=" << widx << ": |u_F|=" << elem(u_F,widx)*elem(u_F,widx) + elem(v_F,widx)*elem(v_F,widx) + elem(w_F,widx)*elem(w_F,widx));

    }

    __common void SetMomentumEquationType(void)
    {
        switch(settings.momentumEquationType)
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

    __common void SetEnergyEquationType(void)
    {
        switch(settings.energyEquationType)
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

    __common void SetTurbulenceEquationType(void)
    {
        switch(settings.turbulenceEquationType)
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
        }
    }
}
