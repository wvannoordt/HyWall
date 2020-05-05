#include "AllEquations.h"
#include "HybridComputing.h"
#include "CoreData.h"
#include "Indexing.h"
#include "DebugTools.h"
namespace HyCore
{
    __common bool EnergyHasJacobian(HyWall::UserSettings* inputSettings)
    {
        return inputSettings->energyEquationType == energy::ODE;
    }
    __common void LinearTInit(const int widx)
    {
        if (settings.adiabaticWall)
        {
            for (int i = 0; i < N; i++) elem(T, widx, i) = elem(T_F, widx);
        }
        else
        {
            for (int i = 0; i < N; i++)
            {
                double t = elem(d, widx, i) / elem(distance, widx);
                elem(T, widx, i) = (1-t)*settings.wallTemperature + t*elem(T_F, widx);
            }
        }
    }

    __common void InitializeEnergy(const int widx)
    {
        LinearTInit(widx);
        for (int i = 0; i < N; i++)
        {
            elem(rho, widx, i) = elem(rho_F, widx);
            elem(mu, widx, i) = elem(mu_F, widx);
        }
    }


    __common void ComputeExplicitEnergyEquation(const int widx, const int engyEq, double* errorOut, double* itsOut)
    {
        switch (engyEq)
        {
            case energy::linear:
            {
                LinearTInit(widx);
                break;
            }
        }
    }

    __common void ComputeLhsRhsEnergy(const int widx)
    {
        switch(settings.energyEquationType)
        {
            case energy::ODE:
            {
                ComputeLhsRhsEnergyODE(widx);
            }
        }
    }

    __common void ComputeLhsRhsEnergyODE(const int widx)
    {
        
    }

    __common void SolveUpdateSystemEnergy(const int widx, double* errorOut)
    {

    }
}
