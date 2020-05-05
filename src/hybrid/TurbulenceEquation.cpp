#include "AllEquations.h"
#include "HybridComputing.h"
#include "CoreData.h"
#include "DebugTools.h"
#include "Indexing.h"
namespace HyCore
{
    __common bool TurbulenceHasJacobian(HyWall::UserSettings* inputSettings)
    {
        return settings.turbulenceEquationType = turbulence::ODE;
    }

    __common void LinearTurbInit(const int widx)
    {
        for (int i = 0; i < N; i++) elem(turb, widx, i) = elem(turb_F, widx)*elem(d, widx, i)/elem(distance, widx);
    }

    __common void InitializeTurbulence(const int widx)
    {
        switch (settings.turbulenceEquationType)
        {
            case turbulence::linear:
            {
                LinearTurbInit(widx);
                break;
            }
            case turbulence::ODE:
            {
                LinearTurbInit(widx);
                //This is where the initial condition correction will go.
                break;
            }
        }
    }

    __common void ComputeExplicitTurbulenceEquation(const int widx, const int turbEq, double* errorOut, double* itsOut)
    {
        switch (turbEq)
        {
            case turbulence::linear:
            {
                LinearTurbInit(widx);
            }
        }
    }

    __common void ComputeLhsRhsTurbulence(const int widx)
    {
        switch(settings.turbulenceEquationType)
        {
            case turbulence::ODE:
            {
                ComputeLhsRhsTurbulenceODE(widx);
            }
        }
    }

    __common void ComputeLhsRhsTurbulenceODE(const int widx)
    {
        
    }

    __common void SolveUpdateSystemTurbulence(const int widx, double* errorOut)
    {

    }
}
