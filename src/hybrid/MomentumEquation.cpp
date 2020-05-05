#include "AllEquations.h"
#include "HybridComputing.h"
#include "CoreData.h"
#include "DebugTools.h"
#include "Indexing.h"
#include "CoreUtils.h"
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
                double dummy1, dummy2;
                ComputeExplicitMomentumEquation(widx, momentum::allmaras, &dummy1, &dummy2);
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

    __common void ComputeLhsRhsMomentum(const int widx)
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

    }

    __common void ComputeLhsRhsMomentumODE(const int widx)
    {
        
    }

    __common double NewtonIterationAllmaras(const int widx, double* errorOut, double* itsOut)
    {
        double uTau = sqrt( (elem(mu_F, widx)/elem(rho_F, widx)) * (elem(u, widx, 2)/elem(d, widx, 2)));
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
        double localError = 1e10;
        double localIts = 0.0;
		double uTau = NewtonIterationAllmaras(widx, &localError, &localIts);
        for (int i = 0; i < N; i++)
        {
            double yPlus = elem(d, widx, i)*uTau*elem(rho, widx, i)/(elem(mu, widx, i));
            elem(u, widx, i) = GetUplusAllmaras(yPlus)*uTau;
        }
        *errorOut += localError;
        *itsOut += localIts;

    }
}
