#include "EquationTypes.h"
#include "AllEquations.h"
#include "TransitionSensorTypes.h"
#include "TransitionSensor.h"
#include "UserSettings.h"
#include "AllocationModes.h"
#include "GlobalMemoryHandler.h"
#include "Variables.h"
#include "ViscousLaws.h"
#include "SolutionProbing.h"
#include "Testing.h"
#include "mpi.h"
#include "HostUtils.h"
#include "ScreenOutput.h"

namespace HyWall
{
    extern UserSettings settings;
    extern GlobalMemoryHandler memory;
    extern TransitionSensor tSensor;
    extern bool restartFileContainedSolutionInit;
    extern "C"
    {
        void Initialize(MPI_Comm host_comm_in, int verboseLevel_in);
        void InitializeInternalMPI(int verboseLevel_in);
        void SetDomainSize(int numWallPoints_in);
        void PassVariable(std::string strname, double* ptr);
        void Allocate(void);
        void DefineVariables(void);
        void PassFlowfieldVariables(double* ptr, int offset);
        void Solve(void);
        void CopySymbols(void);
        void Await(void);
        void SetTimeStep(double timeStep_in);
        void Finalize(void);
        void WhenSettingsAreConstant(void);
        void GetSolutionAndGradient(int widx, int solIdx, double* solOut, double* solGradOut);
        void ReadRestart(int timeStep);
        void WriteRestart(int timeStep);
    }
    void DumpInputState(std::string dirname);
    void DumpOutputState(std::string dirname);
    void DumpPartition(std::string dirname);
}
