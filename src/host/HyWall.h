#include "EnergyEquation.h"
#include "TurbulenceEquation.h"
#include "MomentumEquation.h"
#include "TransitionSensors.h"
#include "UserSettings.h"
#include "AllocationModes.h"
#include "GlobalMemoryHandler.h"
#include "mpi.h"

namespace HyWall
{
    extern UserSettings settings;
    extern GlobalMemoryHandler memory;
    extern "C"
    {
        void Initialize(MPI_Comm host_comm_in, int verboseLevel_in);
        void SetDomainSize(int numWallPoints_in);
        void PassVariable(std::string strname, double* ptr);
        void PassFlowfieldVariables(double* ptr, int offset);
        void Finalize(void);
    }
}
