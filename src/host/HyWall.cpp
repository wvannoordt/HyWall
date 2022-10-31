#include "HyWall.h"
#include "UserSettings.h"
#include "CoreData.h"
#include "AllocationModes.h"
#include "HybridComputing.h"
#include "DebugTools.h"
#include "TransitionSensorTypes.h"
#include "TransitionSensor.h"
#include <string>
#include <vector>
#include "Variables.h"
#include <algorithm>
#include "ScreenOutput.h"
#include "HWPP.hpp"
#include "HyWallCuda.h"
#include "Solver.h"
#include "Typedef.h"
#include "HostUtils.h"
#include <fstream>
#include "SolutionOutput.h"
#include "ViscousLaws.h"
#include "Averaging.h"
#include "SolutionProbing.h"
#include "Testing.h"
#include "IO.h"
#include <sstream>
#include "Indexing.h"
#include "Thermodynamics.h"

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
    double T_f_dns;
    double U_f_dns;
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
    
    double Interpolate(const double& xs, const std::vector<double>& x, const std::vector<double>& y, const int order = 3)
    {
        const int npts = order+1;
        int imin = 0;
        int imax = 0;
        while(imax < x.size()-1 && x[imax] < xs) imax++;
        imin = imax - (order+1)/2;
        if (imin < 0) imin = 0;
        imax = imin + (order);
        double output = 0.0;
        for (int j = imin; j <= imax; ++j)
        {
            double lj  = 1.0;
            for (int i = imin; i <= imax; ++i)
            {
                if (i != j) lj *= (xs - x[i]) / (x[j] - x[i]);
            }
            output += lj*y[j];
        }
        return output;
    }
    
    void ReadFileToVariable(const std::string& var_name, const std::string& filename)
    {
        WriteLine(1, "Reading filename " + filename + " into " + var_name);
        std::vector<double> file_y;
        std::vector<double> file_var;
        std::ifstream ifile (filename);
        std::string line;
        while (std::getline(ifile, line))
        {
            std::istringstream iss(line);
            double y, var;
            iss >> y;
            iss >> var;
            if (iss.fail()) break;
            file_y.push_back(y);
            file_var.push_back(var);
        }
        
        double* y = (double*)memory.GetVariable("sol:d");
        double* v = (double*)memory.GetVariable(var_name);
        const int N = settings.rayDim;
        for (int i = 0; i < memory.localCpuPoints; i++)
        {
            for (int j = 0; j < N; j++)
            {
                double y_val = elem(y, i, j);
                double interp_val = Interpolate(y_val, file_y, file_var);
                elem(v, i, j) = interp_val;
            }
        }
    }
    
    void ReadBufferFiles()
    {
        if (settings.momentumEquationType   == HyCore::momentum::fromFile)
        {
            ReadFileToVariable("sol:u",    settings.momentumFilename);
        }
        if (settings.turbulenceEquationType == HyCore::turbulence::fromFile)
        {
            ReadFileToVariable("sol:mu_t", settings.turbulenceFilename);
        }
        if (settings.energyEquationType     == HyCore::energy::fromFile)
        {
            ReadFileToVariable("sol:T",    settings.energyFilename);
        }
    }
    
    void DumpPartition(std::string dirname)
    {
        WriteLine(1, "DumpPartition [DEBUGGING ONLY!]");
        std::ofstream myfile;
        if (Parallel::isRoot)
        {
            myfile.open(dirname + "/hywallPartition.dat");
            myfile << "pId/cpu/gpu" << std::endl;
        }
        for (int p = 0; p < Parallel::pNum; p++)
        {
            if (p==Parallel::pId)
            {
                std::cout << "p = " << p << ", (C/G)=" << memory.localCpuPoints << "/" << memory.localGpuPoints <<std::endl;
            }
            int numCpuPoints = (p==Parallel::pId)?memory.localCpuPoints:0;
            int numGpuPoints = (p==Parallel::pId)?memory.localGpuPoints:0;
            int numPointsCpuCur = 0;
            int numPointsGpuCur = 0;
            Parallel::Allreduce(&numCpuPoints, &numPointsCpuCur, 1, HY_INT, HY_SUM);
            Parallel::Allreduce(&numGpuPoints, &numPointsGpuCur, 1, HY_INT, HY_SUM);
            Parallel::Sync();
            if (Parallel::isRoot)
            {
                myfile << p << " " << numPointsCpuCur << " " << numPointsGpuCur << std::endl;
            }
            Parallel::Sync();
        }
        if (Parallel::isRoot)
        {
            myfile.close();
        }
    }
    
    template <typename mytype> void DebugOutput(std::string variableName, std::string filename)
    {
        mytype* buf = (mytype*)memory.GetVariable(variableName);
        std::ofstream myfile;
        myfile.open(filename);
        size_t vSize = memory.localCpuPoints;
        for (int i = 0; i < vSize; i++)
        {
            myfile << buf[i] << "\n";
        }
        myfile.close();
    }
    
    void DumpInputState(std::string dirname)
    {
        WriteLine(1, "DumpInputState [DEBUGGING ONLY!]");
        std::vector<std::string> varnames = memory.GetVarsByStartingString("in:");
        for (auto& v:varnames)
        {
            std::string varname = v;
            std::string filename = v;
            std::replace(filename.begin(), filename.end(), ':', '_');
            WriteLine(1, "Output " + v);
            DebugOutput<double>(varname, dirname + "/" + filename + std::to_string(Parallel::pId) + ".dat");
        }
        std::vector<std::string> varnames2 = memory.GetVarsByStartingString("aux:");
        for (auto& v:varnames2)
        {
            std::string varname = v;
            std::string filename = v;
            std::replace(filename.begin(), filename.end(), ':', '_');
            WriteLine(1, "Output " + v);
            DebugOutput<double>(varname, dirname + "/" + filename + std::to_string(Parallel::pId) + ".dat");
        }
    }
    
    void DumpOutputState(std::string dirname)
    {
        WriteLine(1, "DumpOutputState [DEBUGGING ONLY!]");
        std::vector<std::string> varnames = memory.GetVarsByStartingString("out:");
        for (auto& v:varnames)
        {
            std::string varname = v;
            std::string filename = v;
            std::replace(filename.begin(), filename.end(), ':', '_');
            WriteLine(1, "Output " + v);
            DebugOutput<double>(varname, dirname + "/" + filename + std::to_string(Parallel::pId) + ".dat");
        }
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
        const int N = settings.rayDim;
        double* u = (double*)memory.GetVariable("sol:u");
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
                ReadBufferFiles();
            }
            WriteLine(1, "Solve start");
            if (settings.enableTransitionSensor) tSensor.OnEverySolve();
            if (memory.localGpuPoints>0) __withCuda(ComputeGpuSolution());
            if (!((settings.momentumEquationType == HyCore::momentum::fromFile) && (settings.energyEquationType == HyCore::energy::fromFile)))
            {
                for (int i = 0; i < memory.localCpuPoints; i++) HyCore::MainSolver(i);
            }
            else
            {
                double* tau = (double*)memory.GetVariable("out:tau");
                double* qw  = (double*)memory.GetVariable("out:heatflux");
                double* mu  = (double*)memory.GetVariable("sol:mu");
                double*  u  = (double*)memory.GetVariable("sol:u");
                double*  t  = (double*)memory.GetVariable("sol:T");
                double*  uf = (double*)memory.GetVariable("in:u");
                double*  tf = (double*)memory.GetVariable("in:T");
                double tau_DNS;
                double  qw_DNS;
                for (int i = 0; i < memory.localCpuPoints; i++)
                {
                    HyCore::EquationsOfState(i);
                    const double u_DNS = elem(u, i, N-1);
                    const double t_DNS = elem(t, i, N-1);
                    const double u_LES = elem(uf, i);
                    const double t_LES = elem(tf, i);
                    tau_DNS = elem(mu, i, 0)*(elem(u, i, 1)-elem(u, i, 0))/settings.wallSpacing;
                    qw_DNS  = -settings.fluidCp * (elem(mu, i, 0)/settings.fluidPrandtl)*(elem(t, i, 1)-elem(t, i, 0))/settings.wallSpacing;
                    elem(tau, i) = tau_DNS * u_LES / u_DNS;
                    elem(qw,  i) = qw_DNS  * t_LES / t_DNS;
                }
                WriteLine(1, "Enforcing DNS profiles:\ntau:   " + std::to_string(tau_DNS) + "\nqw:    " + std::to_string(qw_DNS));
            }
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
            
            if (settings.verboseLevel >= 4)
            {
                double* tauBuf = (double*)memory.GetVariable("out:tau");
                double* qBuf = (double*)memory.GetVariable("out:heatflux");
                double tauMean = Parallel::GlobalMaxAbs(tauBuf, memory.localTotalPoints);
                double qMax = Parallel::GlobalMaxAbs(qBuf, memory.localTotalPoints);
                WriteLine(4, "Max shear stress: " + to_estring(tauMean));
                WriteLine(4, "Max heatFlux: " + to_estring(-qMax));
            }
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
