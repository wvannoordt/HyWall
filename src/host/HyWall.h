#include "EquationTypes.h"
#include "AllEquations.h"
#include "TransitionSensorTypes.h"
#include "TransitionSensor.h"
#include "UserSettings.h"
#include "AllocationModes.h"
#include "GlobalMemoryHandler.h"
#include "Variables.h"
#include "ViscousLaws.h"
#include "mpi.h"

namespace HyWall
{
    extern UserSettings settings;
    extern GlobalMemoryHandler memory;
    extern TransitionSensor tSensor;
    extern "C"
    {
        void Initialize(MPI_Comm host_comm_in, int verboseLevel_in);
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
    }
}
