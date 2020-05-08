#include "AllEquations.h"
#include "HybridComputing.h"
#include "CoreData.h"
#include "DebugTools.h"
#include "Indexing.h"
#include "CoreUtils.h"
#include "Constants.h"
#if(__cpu)
#include <cmath>
using std::sqrt;
using std::exp;
#endif
namespace HyCore
{
    __common bool TurbulenceHasJacobian(HyWall::UserSettings* inputSettings)
    {
        return inputSettings->turbulenceEquationType == turbulence::ODE;
    }

    __common void LinearTurbInit(const int widx)
    {
        for (int i = 0; i < N; i++)
        {
            elem(mu_t, widx, i) = elem(mu_t_F, widx)*elem(d, widx, i)/elem(distance, widx);
            elem(turb, widx, i) = SaBacksolve(elem(mu_t_F, widx), elem(mu_F, widx), elem(rho_F, widx))*elem(d, widx, i)/elem(distance, widx);
        }
    }

    __common void ZeroTurbInit(const int widx)
    {
        for (int i = 0; i < N; i++) elem(turb, widx, i) = 0.0;
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
        if (settings.enableTransitionSensor && elem(sensorValue, widx)<1.0)
        {
            ZeroTurbInit(widx);
            return;
        }
        switch (turbEq)
        {
            case turbulence::linear:
            {
                LinearTurbInit(widx);
            }
        }
    }

    __common void ComputeVanDriestTurbulence(const int widx)
    {
        double tau = elem(mu, widx, 0)*elem(u, widx, 1)/settings.wallSpacing;
        for (int i = 0; i < N; i++)
        {
            double yPlus = elem(d, widx, i) * sqrt(tau*elem(rho, widx, i))/elem(mu, widx, i);
            double expfactor = 1.0 - exp(-yPlus/17.0);
            elem(mu_t, widx, i) = CONST_SA_KAPPA * elem(mu, widx, i) * yPlus * expfactor*expfactor;
        }
    }

    __common void ComputeAlgebraicTurbulence(const int widx, const int turbEq)
    {
        switch (turbEq)
        {
            case turbulence::vanDriest:
            {
                ComputeVanDriestTurbulence(widx);
                break;
            }
        }
    }

    __common void ComputeLhsRhsTurbulence(const int widx, const double relaxationFactor)
    {
        if (settings.enableTransitionSensor && elem(sensorValue, widx)<1.0)
        {
            ZeroTurbInit(widx);
            return;
        }
        switch(settings.turbulenceEquationType)
        {
            case turbulence::ODE:
            {
                ComputeLhsRhsTurbulenceODE(widx, relaxationFactor);
            }
        }
    }

    __common void ComputeLhsRhsTurbulenceODE(const int widx, const double relaxationFactor)
    {
        for (int i = 1; i < N-1; i++)
        {
            localtriple(uLoc, u, widx, i);
            localtriple(muLoc, mu, widx, i);
            localtriple(turbLoc, turb, widx, i);
            localtriple(rhoLoc, rho, widx, i);
            localtriple(yLoc, d, widx, i);
            dvec3 nuLoc;
            nuLoc[0] = muLoc[0]/rhoLoc[0];
            nuLoc[1] = muLoc[1]/rhoLoc[1];
            nuLoc[2] = muLoc[2]/rhoLoc[2];

            double dy2inv = 1.0 / (0.5*(yLoc[2]-yLoc[0]));
            double dyinvf = 1.0 / (yLoc[2]-yLoc[1]);
            double dyinvb = 1.0 / (yLoc[1]-yLoc[0]);

            double df = 0.5*(nuLoc[1]+nuLoc[2]+turbLoc[1]+turbLoc[2]);
            double db = 0.5*(nuLoc[1]+nuLoc[0]+turbLoc[1]+turbLoc[0]);
            //double t = (yLoc[1]-yLoc[0])/(yLoc[2]-yLoc[0]);
            double t = 0.5;
            double dturb_dy = (1-t)*(turbLoc[1]-turbLoc[0])*dyinvb + t*(turbLoc[2]-turbLoc[1])*dyinvf;
            double du_dy    = (1-t)*(   uLoc[1]-   uLoc[0])*dyinvb + t*(   uLoc[2]-   uLoc[1])*dyinvf;
            double P, D;

            ComputeProductionDestructionSA(&P, &D, rhoLoc[1], muLoc[1], turbLoc[1], yLoc[1], du_dy);

            turbSystem[TD_RHS][i-1]  = dy2inv*(df*(turbLoc[2]-turbLoc[1])*dyinvf - db*(turbLoc[1]-turbLoc[0])*dyinvb)/CONST_SA_SIGMA;
            turbSystem[TD_RHS][i-1] += CONST_SA_CB2 * dturb_dy * dturb_dy / CONST_SA_SIGMA;
            turbSystem[TD_RHS][i-1] += relaxationFactor*(P - D);

            turbSystem[TD_DIA][i-1]  = (-dy2inv*df*dyinvf + dy2inv*(turbLoc[2]-turbLoc[1])*dyinvf*0.5 - dy2inv*db*dyinvb - dy2inv*(turbLoc[1]-turbLoc[0])*dyinvb*0.5)/CONST_SA_SIGMA;
            turbSystem[TD_DIA][i-1] += 2*CONST_SA_CB2 * dturb_dy * ((1-t)*dyinvb - t*dyinvf) / CONST_SA_SIGMA;

            if (i>1)
            {
                turbSystem[TD_SUB][i-2]  = (db*dyinvb + 0.5*(turbLoc[0]-turbLoc[1])*dyinvb)/CONST_SA_SIGMA;
                turbSystem[TD_SUB][i-2] += 2*CONST_SA_CB2*dturb_dy*(t-1)*dyinvb/CONST_SA_SIGMA;
            }

            if (i<N-2)
            {
                turbSystem[TD_SUP][i-1]  = (dy2inv*df*dyinvf + dy2inv*0.5*(turbLoc[2]-turbLoc[1])*dyinvf)/CONST_SA_SIGMA;
                turbSystem[TD_SUP][i-1] += 2*CONST_SA_CB2*dturb_dy*t*dyinvf/CONST_SA_SIGMA;
            }
        }
    }

    __common void SolveUpdateSystemTurbulence(const int widx, double* errorOut)
    {
        *errorOut = 0.0;
        if (settings.enableTransitionSensor && elem(sensorValue, widx)<1.0) return;
        double loc_sq_error = 0.0;
        TDMASolve(turbSystem, N-2);
        for (int i = 0; i < N-2; i++)
        {
            loc_sq_error = turbSystem[TD_RHS][i] / (elem(turb, widx, N-1) + 1e-9);
            *errorOut += loc_sq_error*loc_sq_error;
            elem(turb, widx, i+1) -= settings.turbulenceUnderRelaxationODE*turbSystem[TD_RHS][i];
        }
    }
}
