#include "HybridComputing.h"
#include "Solver.h"
#include "DebugTools.h"
#include <iostream>
#include "Indexing.h"
#include "CoreData.h"
#include "AllEquations.h"
namespace HyCore
{
    __common void MetaDataSet(void)
    {
        SetMomentumEquationType();
        SetEnergyEquationType();
        SetTurbulenceEquationType();
        __dumpwait(energyEquationClassification << ", " << settings.energyEquationType);
    }

    __common void Initialize(const int widx)
    {
        InitializeTurbulence();
        InitializeMomentum();
        InitializeEnergy();
    }

    __common void MainSolver(const int widx)
    {

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
