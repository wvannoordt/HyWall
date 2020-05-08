#include "HyWall.h"
#include "UserSettings.h"
#include "CoreData.h"
#include "AllocationModes.h"
#include "HybridComputing.h"
#include "DebugTools.h"
#include "TransitionSensorTypes.h"
#include "TransitionSensor.h"
#include <string>
#include "Variables.h"
#include "ScreenOutput.h"
#include "HWPP.hpp"
#include "HyWallCuda.h"
#include "Solver.h"
#include "Typedef.h"
#include "HostUtils.h"
namespace HyWall
{
    UserSettings settings;
    GlobalMemoryHandler memory;
    TransitionSensor tSensor;
    bool isFirstSolve;
    double* residualOutput;
    double* iterationsOutput;
    int solveCount;
    void Initialize(MPI_Comm host_comm_in, int verboseLevel_in)
    {
        WriteLine(1, "Initialize");
        HWPP_SetDefaultValues(&settings);
        solveCount = 0;
        memory = GlobalMemoryHandler();
        Parallel::Initialize(host_comm_in);
        isFirstSolve = true;
    }

    void DefineVariables(void)
    {
        DefineInputVariables();
        DefineAuxilaryVariables();
        DefineMomentumVariables();
        DefineEnergyVariables();
        DefineTurbulentVariables();
        DefineOutputVariables();
    }

    void Allocate(void)
    {
        memory.ApplyInitializationPolicies();
        CopySymbols();
    }

    void SetDomainSize(int numWallPoints_in)
    {
        memory.SetSize(numWallPoints_in, settings.rayDim);
        Parallel::MarkActiveRanks(numWallPoints_in>0);
    }

    void SetTimeStep(double timeStep_in)
    {
        settings.timeStep = timeStep_in;
    }

    void PassFlowfieldVariables(double* ptr, int offset)
    {
        memory.SetUserAssociatedVariable("in:p",    ptr+0*offset);
        memory.SetUserAssociatedVariable("in:u",    ptr+1*offset);
        memory.SetUserAssociatedVariable("in:v",    ptr+2*offset);
        memory.SetUserAssociatedVariable("in:w",    ptr+3*offset);
        memory.SetUserAssociatedVariable("in:T",    ptr+4*offset);
        memory.SetUserAssociatedVariable("in:mu_t", ptr+5*offset);
    }

    void CopySymbols(void)
    {
        WriteLine(2, "Copy symbols start");
        if (settings.enableTransitionSensor) tSensor.CopySymbols();
        HyCore::settings = settings;
        HyCore::majorAccessPitch = memory.localCpuPoints;
        HyCore::u            = (double*)memory.GetVariable("sol:u");
        HyCore::d            = (double*)memory.GetVariable("sol:d");
        HyCore::T            = (double*)memory.GetVariable("sol:T");
        HyCore::rho          = (double*)memory.GetVariable("sol:rho");
        HyCore::mu           = (double*)memory.GetVariable("sol:mu");
        HyCore::mu_t         = (double*)memory.GetVariable("sol:mu_t");
        HyCore::turb         = (double*)memory.GetVariable("sol:turb");
        HyCore::p_F          = (double*)memory.GetVariable("in:p");
        HyCore::u_F          = (double*)memory.GetVariable("in:u");
        HyCore::v_F          = (double*)memory.GetVariable("in:v");
        HyCore::w_F          = (double*)memory.GetVariable("in:w");
        HyCore::T_F          = (double*)memory.GetVariable("in:T");
        HyCore::mu_t_F       = (double*)memory.GetVariable("in:mu_t");
        HyCore::distance     = (double*)memory.GetVariable("in:distance");
        HyCore::rho_F        = (double*)memory.GetVariable("in:rho");
        HyCore::mu_F         = (double*)memory.GetVariable("in:mu_lam");

        HyCore::tau          = (double*)memory.GetVariable("out:tau");
        HyCore::iterations   = (double*)memory.GetVariable("out:iterations");
        HyCore::error        = (double*)memory.GetVariable("out:error");
        HyCore::vorticity    = (double*)memory.GetVariable("out:vorticity");
        HyCore::heatflux     = (double*)memory.GetVariable("out:heatflux");

        residualOutput   = (double*)memory.GetVariable("out:error");
        iterationsOutput = (double*)memory.GetVariable("out:iterations");

        if (settings.enableTransitionSensor) HyCore::sensorValue = (double*)memory.GetVariable("aux:sensor_val");

        HyCore::MetaDataSet(&settings);

        if (HyCore::MomentumHasJacobian(&settings))
        {
            HyCore::momSystem[TD_SUB]  = (double*)memory.GetVariable("jac:mom0");
            HyCore::momSystem[TD_DIA]  = (double*)memory.GetVariable("jac:mom1");
            HyCore::momSystem[TD_SUP]  = (double*)memory.GetVariable("jac:mom2");
            HyCore::momSystem[TD_RHS]  = (double*)memory.GetVariable("jac:mom3");
        }

        if (HyCore::TurbulenceHasJacobian(&settings))
        {
            HyCore::turbSystem[TD_SUB] = (double*)memory.GetVariable("jac:turb0");
            HyCore::turbSystem[TD_DIA] = (double*)memory.GetVariable("jac:turb1");
            HyCore::turbSystem[TD_SUP] = (double*)memory.GetVariable("jac:turb2");
            HyCore::turbSystem[TD_RHS] = (double*)memory.GetVariable("jac:turb3");
        }

        if (HyCore::EnergyHasJacobian(&settings))
        {
            HyCore::engySystem[TD_SUB] = (double*)memory.GetVariable("jac:engy0");
            HyCore::engySystem[TD_DIA] = (double*)memory.GetVariable("jac:engy1");
            HyCore::engySystem[TD_SUP] = (double*)memory.GetVariable("jac:engy2");
            HyCore::engySystem[TD_RHS] = (double*)memory.GetVariable("jac:engy3");
        }
        if (memory.localGpuPoints>0)
        {
            __withCuda(WriteLine(2, "Copy CUDA symbols start"));
            __withCuda(CopyCudaSymbols());
        }
        WriteLine(2, "Copy symbols end");
    }

    void PassVariable(std::string strname, double* ptr)
    {
        memory.SetUserAssociatedVariable(strname, ptr);
    }

    void Solve(void)
    {
        if (solveCount++ % settings.solveSkip == 0)
        {
            if (isFirstSolve)
            {
                if (settings.enableTransitionSensor) tSensor.OnFirstSolve();
                WriteLine(1, "Initializing wall model solution");
                if (memory.localGpuPoints>0) __withCuda(InitGpuSolution());
                for (int i = 0; i < memory.localCpuPoints; i++) HyCore::Initialize(i);
            }
            WriteLine(1, "Solve start");
            if (settings.enableTransitionSensor) tSensor.OnEverySolve();
            if (memory.localGpuPoints>0) __withCuda(ComputeGpuSolution());
            for (int i = 0; i < memory.localCpuPoints; i++) HyCore::MainSolver(i);

            double meanIts = Parallel::GlobalAverageAbs(iterationsOutput, memory.localTotalPoints);
            double totError = Parallel::GlobalTotalAbs(residualOutput, memory.localTotalPoints);
            double maxError = Parallel::GlobalMaxAbs(residualOutput, memory.localTotalPoints);
            WriteLine(1, "Solve end, residual total: " + to_estring(totError) + ", residual max:" + to_estring(maxError) + ", mean iterations: " + to_estring(meanIts));
            isFirstSolve = false;
        }
    }

    void Finalize(void)
    {
        WriteLine(1, "Closing");
        memory.ApplyFinalizationPolicies();
        Parallel::Finalize();
        WriteLine(1, "Closed");
    }
}
