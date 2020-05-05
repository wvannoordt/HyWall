#include "HybridComputing.h"
#include "AllEquations.h"
#include "CoreData.h"
namespace HyCore
{
    __common void ComputeExplicitExpressions(const int widx)
    {
        if (momentumEquationClassification == equationType::explicitEq)
        {
            ComputeExplicitMomentumEquation(widx, settings.momentumEquationType);
        }
        if (turbulenceEquationClassification == equationType::explicitEq)
        {
            ComputeExplicitTurbulenceEquation(widx, settings.turbulenceEquationType);
        }
        if (energyEquationClassification == equationType::explicitEq)
        {
            ComputeExplicitEnergyEquation(widx, settings.energyEquationType);
        }
    }
}
