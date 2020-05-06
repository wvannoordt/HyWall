#include "HybridComputing.h"
#include "AllEquations.h"
#include "CoreData.h"
#include "DebugTools.h"
namespace HyCore
{
    __common void ComputeExplicitExpressions(const int widx, double* errorOut, double* itsOut)
    {
        double localError = 0;
        double localIts = 0;
        if (momentumEquationClassification == equationType::explicitEq)
        {
            ComputeExplicitMomentumEquation(widx, settings.momentumEquationType, &localError, &localIts);
            *errorOut += localError;
            *itsOut += localIts;
        }
        if (turbulenceEquationClassification == equationType::explicitEq)
        {
            ComputeExplicitTurbulenceEquation(widx, settings.turbulenceEquationType, &localError, &localIts);
            *errorOut += localError;
            *itsOut += localIts;
        }
        if (energyEquationClassification == equationType::explicitEq)
        {
            ComputeExplicitEnergyEquation(widx, settings.energyEquationType, &localError, &localIts);
            *errorOut += localError;
            *itsOut += localIts;
        }
    }


    __common void ComputeLinearSystems(const int widx, const double relaxationFactor)
    {
        if (momentumEquationClassification == equationType::differentialEq)
        {
            ComputeLhsRhsMomentum(widx, relaxationFactor);
        }
        if (turbulenceEquationClassification == equationType::differentialEq)
        {
            ComputeLhsRhsTurbulence(widx, relaxationFactor);
        }
        if (energyEquationClassification == equationType::differentialEq)
        {
            ComputeLhsRhsEnergy(widx, relaxationFactor);
        }
    }

    __common void SolveUpdateLinearSystems(const int widx, double* errorOut)
    {
        double errorLocal = 0.0;
        double temp;
        if (momentumEquationClassification == equationType::differentialEq)
        {
            SolveUpdateSystemMomentum(widx, &temp);
            errorLocal += temp;
        }
        if (turbulenceEquationClassification == equationType::differentialEq)
        {
            SolveUpdateSystemTurbulence(widx, &temp);
            errorLocal += temp;
        }
        if (energyEquationClassification == equationType::differentialEq)
        {
            SolveUpdateSystemEnergy(widx, &temp);
            errorLocal += temp;
        }
        *errorOut = errorLocal/3;
    }


    __common void ComputeAlgebraicExpressions(const int widx)
    {
        if (momentumEquationClassification == equationType::algebraicEq)
        {
            __erkill("NOT YET IMPLEMENTED");
        }
        if (turbulenceEquationClassification == equationType::algebraicEq)
        {
            __erkill("NOT YET IMPLEMENTED");
        }
        if (energyEquationClassification == equationType::algebraicEq)
        {
            __erkill("NOT YET IMPLEMENTED");
        }
    }
}
