#include "HybridComputing.h"
#include "Solver.h"
#include "DebugTools.h"
#include <iostream>
#include <fstream>
#include "Indexing.h"
#include "CoreData.h"
#include "AllEquations.h"
#include "CoreUtils.h"
#include "Thermodynamics.h"
#include "Parallel.h"
#if(___cpu)
#include <cmath>
using std::sqrt;
#endif
namespace HyCore
{
    __common void GetSolutionAndGradient(int widx, int solIdx, double* solOut, double* solGradOut)
    {
        localtriple(uLoc, u, widx, solIdx);
        localtriple(muLoc, mu, widx, solIdx);
        localtriple(yLoc, d, widx, solIdx);
        localtriple(mutLoc, mu_t, widx, solIdx);
        localtriple(rhoLoc, rho, widx, solIdx);
        localtriple(TLoc, T, widx, solIdx);
        *(solOut+0) = TLoc[1];
        *(solOut+1) = rhoLoc[1];
        *(solOut+2) = muLoc[1];
        *(solOut+3) = mutLoc[1];
        *(solOut+4) = uLoc[1];
        double alpha = (yLoc[1]-yLoc[0]) / (yLoc[2]-yLoc[0]);
        *(solGradOut+0) = (1.0-alpha)*((TLoc[1]  -TLoc[0]  )/(yLoc[1]-yLoc[0])) + alpha*((TLoc[2]  -TLoc[1]  )/(yLoc[2]-yLoc[1]));
        *(solGradOut+1) = (1.0-alpha)*((rhoLoc[1]-rhoLoc[0])/(yLoc[1]-yLoc[0])) + alpha*((rhoLoc[2]-rhoLoc[1])/(yLoc[2]-yLoc[1]));
        *(solGradOut+2) = (1.0-alpha)*((muLoc[1] -muLoc[0] )/(yLoc[1]-yLoc[0])) + alpha*((muLoc[2] -muLoc[1] )/(yLoc[2]-yLoc[1]));
        *(solGradOut+3) = (1.0-alpha)*((mutLoc[1]-mutLoc[0])/(yLoc[1]-yLoc[0])) + alpha*((mutLoc[2]-mutLoc[1])/(yLoc[2]-yLoc[1]));
        *(solGradOut+4) = (1.0-alpha)*((uLoc[1]  -uLoc[0]  )/(yLoc[1]-yLoc[0])) + alpha*((uLoc[2]  -uLoc[1]  )/(yLoc[2]-yLoc[1]));
        
    }
    __common void MetaDataSet(HyWall::UserSettings* inputSettings)
    {
        SetMomentumEquationType(inputSettings);
        SetEnergyEquationType(inputSettings);
        SetTurbulenceEquationType(inputSettings);
        N = inputSettings->rayDim;
    }

    __common void BuildGrid(const int widx)
    {
        double growthRate = getGrowthRate(0, elem(distance, widx), settings.wallSpacing, N, settings.errorTolerance, settings.maxIterations);
        elem(d, widx, 0) = 0.0;
        elem(d, widx, 1) = settings.wallSpacing;
        elem(d, widx, N-1) = elem(distance, widx);
        for (int i = 2; i < N-1; i++)
        {
            elem(d, widx, i) = elem(d, widx, i-1) + growthRate*(elem(d, widx, i-1)-elem(d, widx, i-2));
        }
    }

    __common void InitializeBuffers(const int widx)
    {
        LinearTInit(widx);
        LinearUInit(widx);
        LinearTurbInit(widx);
        for (int i = 0; i < N; i++)
        {
            elem(rho, widx, i) = elem(rho_F, widx);
            elem(mu, widx, i) = elem(mu_F, widx);
            elem(mu_t, widx, i) = elem(mu_t_F, widx);
        }
    }

    __common void Initialize(const int widx)
    {
        BuildGrid(widx);
        InitializeBuffers(widx);
        UpdateBoundaryConditions(widx);
        InitializeEnergy(widx);
        InitializeTurbulence(widx);
        InitializeMomentum(widx);
        EquationsOfState(widx);
    }

    __common void UpdateBoundaryConditions(const int widx)
    {
        elem(u,    widx, N-1) = elem(u_F,    widx);
        elem(T,    widx, N-1) = elem(T_F,    widx);
        elem(mu_t, widx, N-1) = elem(mu_t_F, widx);
        elem(turb, widx, N-1) = SaBacksolve(elem(mu_t_F, widx), elem(mu_F, widx), elem(rho_F, widx));
        elem(rho,  widx, N-1) = elem(rho_F,  widx);
        elem(mu,   widx, N-1) = elem(mu_F,   widx);
    }

    __common void MainSolver(const int widx)
    {
        bool done = false;
        double totalError = 0.0;
        double totalIts = 0.0;
        double localError = 0.0;
        double localIts = 0.0;
        double dummy = 0.0;
        int numIts = 0;
        double relaxationFactor = 0.2;
        double growth = 1.1;

        UpdateBoundaryConditions(widx);
        EquationsOfState(widx);
        ComputeExplicitExpressions(widx, &localError, &dummy);
        if (settings.enableTransitionSensor && elem(sensorMult, widx)<0.5) ZeroTurbInit(widx);
        totalError += localError;
        totalIts += localIts;
        localError = 100000;
        localIts = 0.0;
        elem(failurelevel, widx) = 0.0;
        if (settings.includeMomentumRhs)
        {
            double dummy1 = 0;
            double dummy2 = 0;
            ComputeAllmarasMomentumToTargetBuffer(widx, &dummy2, &dummy1, u_SA);
        }

        while ((d_abs(localError) > settings.errorTolerance) && (numIts < settings.maxIterations))
        {
            ComputeLinearSystems(widx, relaxationFactor);
            SolveUpdateLinearSystems(widx, &localError);
            EquationsOfState(widx);
            ComputeAlgebraicExpressions(widx);
            localIts+=1.0;
            relaxationFactor *= growth;
            relaxationFactor = (relaxationFactor>1.0)?1.0:relaxationFactor;
            numIts++;
        }
        totalError += localError;
        totalIts += localIts;

        double u1  = elem(u, widx, 1);
        double mu1 = elem(mu, widx, 0);
        elem(error, widx) = totalError;
        elem(iterations, widx) = numIts;
        elem(tau, widx) = mu1*u1/settings.wallSpacing;
        elem(vorticity, widx) = u1/settings.wallSpacing;
	double qw = -(settings.fluidCp*mu1/settings.fluidPrandtl)*(elem(T, widx, 1)-elem(T, widx, 0))/settings.wallSpacing;
        elem(heatflux, widx) = qw;

        if (d_abs(localError) > settings.errorTolerance || totalError != totalError || localError != localError)
        {
            OnFailedSolve(widx, localError, numIts);
        }
    }

    __common void OnFailedSolve(int widx, double localError, int numIts)
    {
        double u1  = elem(u, widx, 0);
        double mu1 = PowerLawViscosityRelation(settings.wallTemperature);
        double umag = sqrt(elem(u_F,widx)*elem(u_F,widx) + elem(v_F,widx)*elem(v_F,widx) + elem(w_F,widx)*elem(w_F,widx));
        if (settings.laminarOnSolveFail)
        {
            LinearUInit(widx);
            LinearTInit(widx);
            u1  = elem(u, widx, 1);
            elem(error, widx) = 0;
            elem(iterations, widx) = 0;
	    double tauw = elem(mu_F, widx)*umag/elem(distance, widx);
	    double qw   = -(settings.fluidCp*mu1/settings.fluidPrandtl)*(elem(T_F,widx)-settings.wallTemperature)/elem(distance, widx);
            elem(tau, widx) = tauw;
            elem(vorticity, widx) = u1/settings.wallSpacing;
            elem(heatflux, widx) = qw;
            elem(failurelevel, widx) = 1.0;
	    if (qw!=qw)
	      {
		std::cout << mu1 << ", " << elem(T_F,widx) << std::endl;
		__erkill("AAAAA");
	      }
        }
        else
        {
            __qdump("Wall model solve failed. Data:");
            __qdump("PID    = " << HyWall::Parallel::pId);
            __qdump("widx   = " << widx);
            __qdump("|u_F|  = " << umag);
            __qdump("u_F    = (" << elem(u_F, widx) << ", " << elem(v_F, widx) << ", " << elem(w_F, widx) << ")");
            __qdump("x      = (" << elem(x, widx) << ", " << elem(y, widx) << ", " << elem(z, widx) << ")");
            __qdump("error  = " << localError);
            __qdump("numIts = " << numIts);
            __qdump("T_F    = " << elem(T_F, widx));
            __qdump("mu_t_F = " << elem(mu_t_F, widx));
            __qdump("rho_F  = " << elem(rho_F, widx));
            __qdump("tau    = " << elem(tau, widx));
            __qdump("mu1    = " << mu1);
            __qdump("u1     = " << u1);
            __qdump("q      = " << elem(heatflux, widx));
            __qdump("dx0    = " << settings.wallSpacing);
            if (settings.includeMomentumRhs)
            {
                __qdump("MomRHS = " << elem(momBalancedRHS, widx));
                __qdump("dpdx   = " << elem(dpdx, widx));
            }
            __qdump("Wall Model Solution Buffer [y, u, mu_t, T]:");
            for (int i = 0; i < N; i++)
            {
                __qdump(elem(d, widx, i) << ", " << elem(u, widx, i) << ", " << elem(mu_t, widx, i) << ", " << elem(T, widx, i));
            }
            __erkill("stopping");
        }
    }

    __common void SetMomentumEquationType(HyWall::UserSettings* inputSettings)
    {
        switch(inputSettings->momentumEquationType)
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

    __common void SetEnergyEquationType(HyWall::UserSettings* inputSettings)
    {
        switch(inputSettings->energyEquationType)
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

    __common void SetTurbulenceEquationType(HyWall::UserSettings* inputSettings)
    {
        switch(inputSettings->turbulenceEquationType)
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
            case turbulence::vanDriest:
            {
                turbulenceEquationClassification = equationType::algebraicEq;
                break;
            }
            case turbulence::pnlm:
            {
                turbulenceEquationClassification = equationType::algebraicEq;
                break;
            }
        }
    }
}
