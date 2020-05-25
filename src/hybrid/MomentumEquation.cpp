#include "AllEquations.h"
#include "HybridComputing.h"
#include "CoreData.h"
#include "DebugTools.h"
#include "Indexing.h"
#include "CoreUtils.h"
#include "Typedef.h"
#if (__cpu)
#include <cmath>
using std::sqrt;
#endif
namespace HyCore
{
    __common bool MomentumHasJacobian(HyWall::UserSettings* inputSettings)
    {
        return inputSettings->momentumEquationType == momentum::ODE;
    }

    __common void LinearUInit(const int widx)
    {
        for (int i = 0; i < N; i++)
        {
            double t = elem(d, widx, i)/elem(distance, widx);
            elem(u, widx, i) = t*elem(u_F, widx);
        }
    }

    __common void InitializeMomentum(const int widx)
    {
        switch (settings.momentumEquationType)
        {
            case momentum::allmaras:
            {
                LinearUInit(widx);
                break;
            }
            case momentum::ODE:
            {
                LinearUInit(widx);
                double dummy1 = 0.0;
                double dummy2 = 0.0;
                ComputeAllmarasMomentum(widx, &dummy1, &dummy2);
                break;
            }
        }
    }

    __common void ComputeExplicitMomentumEquation(const int widx, const int momEq, double* errorOut, double* itsOut)
    {
        switch(settings.momentumEquationType)
        {
            case momentum::allmaras:
            {
                ComputeAllmarasMomentum(widx, errorOut, itsOut);
                break;
            }
        }
    }

    __common void ComputeLhsRhsMomentum(const int widx, const double relaxationFactor)
    {
        switch(settings.momentumEquationType)
        {
            case momentum::ODE:
            {
                ComputeLhsRhsMomentumODE(widx);
            }
        }
    }

    __common void SolveUpdateSystemMomentum(const int widx, double* errorOut)
    {
        *errorOut = 0.0;
        double loc_sq_error = 0.0;
        TDMASolve(momSystem, N-2);
        for (int i = 0; i < N-2; i++)
        {
            loc_sq_error = momSystem[TD_RHS][i] / (elem(u_F, widx)+1e-9);
            *errorOut += loc_sq_error*loc_sq_error;
            elem(u, widx, i+1) -= settings.momentumUnderRelaxationODE*momSystem[TD_RHS][i];
        }
    }

    __common void ComputeLhsRhsMomentumODE(const int widx)
    {
        double iTurb = 1.0;
        if (settings.enableTransitionSensor) iTurb = elem(sensorMult, widx);
        for (int i = 1; i < N-1; i++)
        {
            localtriple(uLoc, u, widx, i);
            localtriple(muLoc, mu, widx, i);
            localtriple(yLoc, d, widx, i);
            localtriple(mutLoc, mu_t, widx, i);

            double dy2inv = 1.0 / (0.5*(yLoc[2]-yLoc[0]));
            double dyinvf = 1.0 / (yLoc[2]-yLoc[1]);
            double dyinvb = 1.0 / (yLoc[1]-yLoc[0]);


            double df = 0.5*(muLoc[1]+muLoc[2]+iTurb*mutLoc[1]+iTurb*mutLoc[2]);
            double db = 0.5*(muLoc[1]+muLoc[0]+iTurb*mutLoc[1]+iTurb*mutLoc[0]);
            momSystem[TD_RHS][i-1]  = dy2inv*(df*(uLoc[2]-uLoc[1])*dyinvf - db*(uLoc[1]-uLoc[0])*dyinvb);
            if (settings.includeMomentumRhs) momSystem[TD_RHS][i-1] -= (elem(dpdx, widx) + elem(momBalancedRHS, widx)*elem(u_SA, widx, i)/elem(u_F, widx));
            momSystem[TD_DIA][i-1] = -dy2inv*(df*dyinvf + db*dyinvb);
            if (i>1)   momSystem[TD_SUB][i-2] = dy2inv*db*dyinvb;
            if (i<N-2) momSystem[TD_SUP][i-1] = dy2inv*df*dyinvf;
        }
    }

    __common double NewtonIterationAllmaras(const int widx, double* errorOut, double* itsOut)
    {
        double uTau = sqrt( (elem(mu_F, widx)/elem(rho_F, widx)) * (elem(u, widx, 2)/elem(d, widx, 2)));
        double initGuess = uTau;
        double epsilon = 100;
        double uPlus = 0.0;
        double yPlus = 0.0;
        double f;
        double fprime;
        int k = 0;
        while (d_abs(epsilon) > settings.errorTolerance && k++ < settings.maxIterations)
        {
            yPlus = uTau * elem(d, widx, N-1) * elem(rho_F, widx) / elem(mu, widx, N-1);
            uPlus = GetUplusAllmaras(yPlus);
            f = (elem(u_F, widx)/(uTau))-uPlus;
            fprime = -elem(u_F, widx)/((uTau)*(uTau));
            epsilon = -f/fprime;
            uTau = uTau + 0.9*epsilon;
        }
        *errorOut = d_abs(epsilon);
        *itsOut = (double)k;
        return uTau;
    }

    __common void ComputeAllmarasMomentum(const int widx, double* errorOut, double* itsOut)
    {
        ComputeAllmarasMomentumToTargetBuffer(widx, errorOut, itsOut, u);
    }

    __common void ComputeAllmarasMomentumToTargetBuffer(const int widx, double* errorOut, double* itsOut, double* targetBuffer)
    {
        double localError = 1e10;
        double localIts = 0.0;
		double uTau = NewtonIterationAllmaras(widx, &localError, &localIts);
        for (int i = 0; i < N; i++)
        {
            double yPlus = elem(d, widx, i)*uTau*elem(rho, widx, i)/(elem(mu, widx, i));
            elem(targetBuffer, widx, i) = GetUplusAllmaras(yPlus)*uTau;
        }
        *errorOut += localError;
        *itsOut += localIts;
    }
}
