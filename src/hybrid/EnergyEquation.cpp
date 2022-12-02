#include "AllEquations.h"
#include "TurbulenceEquation.h"
#include "HybridComputing.h"
#include "CoreData.h"
#include "Indexing.h"
#include "DebugTools.h"
#include "CoreUtils.h"
#if(___cpu)
#include <cmath>
using std::sqrt;
using std::exp;
#endif
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
            case energy::fromFile:
            {
                break;
            }
        }
    }

    __common void ComputeLhsRhsEnergy(const int widx, const double relaxationFactor)
    {
        switch(settings.energyEquationType)
        {
            case energy::ODE:
            {
                ComputeLhsRhsEnergyODE(widx);
            }
            case energy::fromFile:
            {
                break;
            }
        }
    }

    __common void ComputeLhsRhsEnergyODE(const int widx)
    {
        double Pr = settings.fluidPrandtl;
        double Pr_t = settings.turbPradntl;
        double Cp = settings.fluidCp;
        double iTurb = 1.0;
        if (settings.enableTransitionSensor) iTurb = elem(sensorMult, widx);

        for (int i = 1; i < N-1; i++)
        {
            localtriple(uLoc, u, widx, i);
            localtriple(muLoc, mu, widx, i);
            localtriple(mutLoc, mu_t, widx, i);
            localtriple(lamtLoc, lam_t, widx, i);
            localtriple(yLoc, d, widx, i);
            localtriple(TLoc, T, widx, i);

            double dy2inv = 1.0 / (0.5*(yLoc[2]-yLoc[0]));
            double dyinvf = 1.0 / (yLoc[2]-yLoc[1]);
            double dyinvb = 1.0 / (yLoc[1]-yLoc[0]);

            double df_engy = 0.5*(Cp*(muLoc[1]+muLoc[2])/Pr+iTurb*(lamtLoc[1]+lamtLoc[2]));
            double db_engy = 0.5*(Cp*(muLoc[1]+muLoc[0])/Pr+iTurb*(lamtLoc[1]+lamtLoc[0]));

            double df_mom = 0.5*((muLoc[2]+iTurb*mutLoc[2])*uLoc[2] + (muLoc[1]+iTurb*mutLoc[1])*uLoc[1]);
            double db_mom = 0.5*((muLoc[0]+iTurb*mutLoc[0])*uLoc[0] + (muLoc[1]+iTurb*mutLoc[1])*uLoc[1]);

            engySystem[TD_RHS][i-1]  = dy2inv*(df_engy*(TLoc[2]-TLoc[1])*dyinvf - db_engy*(TLoc[1]-TLoc[0])*dyinvb) + dy2inv*(df_mom*(uLoc[2]-uLoc[1])*dyinvf - db_mom*(uLoc[1]-uLoc[0])*dyinvb);
            // engySystem[TD_RHS][i-1]  += elem(rho, widx, i)*elem(u, widx, i)*20.75/0.498742216e-02;
            engySystem[TD_DIA][i-1] = -dy2inv*(df_engy*dyinvf + db_engy*dyinvb);

            if (i>1)   engySystem[TD_SUB][i-2] = dy2inv*db_engy*dyinvb;
            if (i<N-2) engySystem[TD_SUP][i-1] = dy2inv*df_engy*dyinvf;
        }
    }

    __common void SolveUpdateSystemEnergy(const int widx, double* errorOut)
    {
        *errorOut = 0.0;
        double loc_sq_error = 0.0;
        TDMASolve(engySystem, N-2);
        for (int i = 0; i < N-2; i++)
        {
            loc_sq_error = engySystem[TD_RHS][i]/(elem(T_F, widx)+1e-9);
            *errorOut += loc_sq_error*loc_sq_error;
            elem(T, widx, i+1) -= settings.energyUnderRelaxationODE*engySystem[TD_RHS][i];
        }
        if (settings.adiabaticWall) elem(T, widx, 0) -= settings.energyUnderRelaxationODE*engySystem[TD_RHS][0];
    }
}
