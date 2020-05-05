#include "AllEquations.h"
#include "HybridComputing.h"
#include "CoreData.h"
#include "Indexing.h"
namespace HyCore
{
    __common void InitializeTurbulence(const int widx)
    {
        switch (settings.turbulenceEquationType)
        {
            case turbulence::linear:
            {
                for (int i = 0; i < N; i++) elem(turb, widx, i) = elem(turb_F, widx)*elem(d, widx, i)/elem(distance, widx);
                break;
            }
            case turbulence::ODE:
            {
                for (int i = 0; i < N; i++) elem(turb, widx, i) = elem(turb_F, widx)*elem(d, widx, i)/elem(distance, widx);
                break;
            }
        }
    }

    __common void ComputeExplicitTurbulenceEquation(const int widx, const int momEq)
    {

    }
}
