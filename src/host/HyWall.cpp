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
#include "SolutionOutput.h"
#include "ViscousLaws.h"
#include "Averaging.h"
#include "SolutionProbing.h"
#include "Testing.h"
#include "IO.h"
namespace HyWall
{
    UserSettings settings;
    GlobalMemoryHandler memory;
    TransitionSensor tSensor;
    bool isFirstSolve;
    bool hasInitialized;
    bool restartFileContainedSolutionInit;
    double* residualOutput;
    double* iterationsOutput;
    double* failuresOutput;
    double* xInput;
    int solveCount;
    void Initialize(MPI_Comm host_comm_in, int verboseLevel_in)
    {
        WriteLine(1, "Initialize");
        HWPP_SetDefaultValues(&settings);
        solveCount = 0;
        memory = GlobalMemoryHandler();
        Parallel::Initialize(host_comm_in);
        isFirstSolve = true;
        Testing::testingMode = false;
        InitProbeIndex();
        //settings are still volatile here.
        hasInitialized = true;
        restartFileContainedSolutionInit = false;
    }
    void InitializeInternalMPI(int verboseLevel_in)
    {
        MPI_Init(NULL, NULL);
        Parallel::Initialize(MPI_COMM_WORLD);
        Initialize(MPI_COMM_WORLD, verboseLevel_in);
        Parallel::internalMPIHandling = true;
    }

    void WhenSettingsAreConstant(void)
    {
        if (settings.IOEnable) InitializeSolutionOutput();
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
        if (settings.averageSolution) InitializeAveraging();
        memory.ApplyInitializationPolicies();
        CopySymbols();
        WhenSettingsAreConstant();
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

    void Await(void)
    {

    }

    void CopySymbols(void)
    {
        WriteLine(2, "Copy symbols start");
        if (settings.enableTransitionSensor) tSensor.CopySymbols();
        HyCoreCPU::settings = settings;
        HyCoreCPU::majorAccessPitch = memory.localCpuPoints;
        HyCoreCPU::MetaDataSet(&settings);
        residualOutput   = (double*)memory.GetVariable("out:error");
        iterationsOutput = (double*)memory.GetVariable("out:iterations");
        xInput           = (double*)memory.GetVariable("in:x");
        failuresOutput   = (double*)memory.GetVariable("out:failurelevel");
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
        if (isFirstSolve && settings.averageSolution) InitializeAveraging();
        if (solveCount++ % settings.solveSkip == 0)
        {
            HyCoreCPU::solveCount = solveCount;
            bool isReastartAndHasBeenInitialized = false;// = restartFileContainedSolutionInit&&settings.readRestart;
            if ((isFirstSolve || settings.alwaysReinitialize) && !isReastartAndHasBeenInitialized)
            {
                if (settings.enableTransitionSensor&&isFirstSolve) tSensor.OnFirstSolve();
                WriteLine(2, "Initializing wall model solution");
                if (memory.localGpuPoints>0) __withCuda(InitGpuSolution());
                for (int i = 0; i < memory.localCpuPoints; i++) HyCore::Initialize(i);
            }
            WriteLine(1, "Solve start");
            if (settings.enableTransitionSensor) tSensor.OnEverySolve();
            if (memory.localGpuPoints>0) __withCuda(ComputeGpuSolution());
            for (int i = 0; i < memory.localCpuPoints; i++) HyCore::MainSolver(i);

            double meanIts = Parallel::GlobalAverageAbs(iterationsOutput, memory.localTotalPoints);
            int maxIts = Parallel::GlobalMaxAbs(iterationsOutput, memory.localTotalPoints);
            double maxError = Parallel::GlobalMaxAbs(residualOutput, memory.localTotalPoints);
            if (settings.laminarOnSolveFail)
            {
                double xMinFail, xMaxFail;
                double totalFailedPoints = Parallel::GlobalSum(failuresOutput, memory.localTotalPoints);
                Parallel::GlobalBoundsProduct(failuresOutput, xInput, memory.localTotalPoints, &xMinFail, &xMaxFail);
                if (totalFailedPoints > 0.5) WriteLine(1, "Detected failure. Total: " + std::to_string(totalFailedPoints) + ", xmin/xmax: " + std::to_string(xMinFail) + ", " + std::to_string(xMaxFail));
            }
            WriteLine(1, "Solve end, residual max:" + to_estring(maxError) + ", mean iterations: " + to_estring(meanIts));
            WriteLine(4, "Max iterations: " + to_estring(maxIts));
            
            double* tauBuf = (double*)memory.GetVariable("out:tau");
            double tauMean = Parallel::GlobalMaxAbs(tauBuf, memory.localTotalPoints);
            WriteLine(4, "Max shear stress: " + to_estring(tauMean));
            isFirstSolve = false;
        }
        if (settings.averageSolution)
        {
            ComputeAverages();
            if (solveCount%settings.averageOutputInterval==0)
            {
                SaveAveragesToFile(solveCount);
            }
        }
    }
    
    void GetSolutionAndGradient(int widx, int solIdx, double* solOut, double* solGradOut)
    {
        HyCore::GetSolutionAndGradient(widx, solIdx, solOut, solGradOut);
    }

    void ReadRestart(int timeStep)
    {
        if (!settings.enableWallModel) return;
        if (!settings.readRestart) return;
        std::string prefix = "restart";
        char buf[100] = {0};
        std::snprintf(buf, sizeof(buf), "%08d", timeStep);
        std::string iname = buf;
        std::string filename = prefix + "/HyWallRestart_nt_" + iname + ".rst";
        WriteLine(1, "wall model restart read: " + filename);
        IO::ReadState(filename);
    }

    void WriteRestart(int timeStep)
    {
        if (!settings.enableWallModel) return;
        std::string prefix = "restart";
        char buf[100] = {0};
        std::snprintf(buf, sizeof(buf), "%08d", timeStep);
        std::string iname = buf;
        std::string filename = prefix + "/HyWallRestart_nt_" + iname + ".rst";
        WriteLine(1, "wall model restart write: " + filename);
        IO::WriteStateByFlags(filename, bflag::restorable | bflag::solution);
    }

    void Finalize(void)
    {
        WriteLine(1, "Closing");
        memory.ApplyFinalizationPolicies();
        Parallel::Finalize();
        if (Parallel::internalMPIHandling) MPI_Finalize();
        if (settings.averageSolution) FinalizeAveraging();
        WriteLine(1, "Closed");
    }
}
