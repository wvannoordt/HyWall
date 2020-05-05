#include "HyWall.h"
#include "UserSettings.h"
#include "CoreData.h"
#include "AllocationModes.h"
#include "HybridComputing.h"
#include "DebugTools.h"
#include "TransitionSensors.h"
#include <string>
#include "Variables.h"
#include "ScreenOutput.h"
#include "HWPP.hpp"
#include "HyWallCuda.h"
#include "Solver.h"
namespace HyWall
{
    UserSettings settings;
    GlobalMemoryHandler memory;
    bool isFirstSolve;
    void Initialize(MPI_Comm host_comm_in, int verboseLevel_in)
    {
        WriteLine(1, "Initialize HyWall");
        HWPP_SetDefaultValues(&settings);
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
        memory.SetUserAssociatedVariable("in:turb", ptr+5*offset);
    }

    void CopySymbols(void)
    {
        WriteLine(2, "Copy symbols start");
        HyCore::settings = settings;
        HyCore::majorAccessPitch = memory.localCpuPoints;
        HyCore::u      = (double*)memory.GetVariable("sol:u");
        HyCore::d      = (double*)memory.GetVariable("sol:d");
        HyCore::T      = (double*)memory.GetVariable("sol:T");
        HyCore::rho    = (double*)memory.GetVariable("sol:rho");
        HyCore::mu     = (double*)memory.GetVariable("sol:mu");
        HyCore::turb   = (double*)memory.GetVariable("sol:turb");
        HyCore::p_F    = (double*)memory.GetVariable("in:p");
        HyCore::u_F    = (double*)memory.GetVariable("in:u");
        HyCore::v_F    = (double*)memory.GetVariable("in:v");
        HyCore::w_F    = (double*)memory.GetVariable("in:w");
        HyCore::T_F    = (double*)memory.GetVariable("in:T");
        HyCore::turb_F = (double*)memory.GetVariable("in:turb");
        HyCore::MetaDataSet();
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
        if (isFirstSolve)
        {
            WriteLine(1, "Initializing wall model solution");
            if (memory.localGpuPoints>0) __withCuda(InitGpuSolution());
            for (int i = 0; i < memory.localCpuPoints; i++) HyCore::Initialize(i);
        }
        for (int i = 0; i < memory.localCpuPoints; i++) HyCore::MainSolver(i);
        isFirstSolve = false;
    }

    void Finalize(void)
    {
        memory.ApplyFinalizationPolicies();
        Parallel::Finalize();
        WriteLine(1, "Close HyWall");
    }
}
