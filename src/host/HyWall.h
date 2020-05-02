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
        void SetComm(MPI_Comm host_comm_in);
        void Initialize(int numWallPoints_in);
        void Finalize(void);
    }
}
