#include "HyWall.h"
namespace HyWall
{
    UserSettings settings;
    GlobalMemoryHandler memory;
    void SetComm(MPI_Comm host_comm_in)
    {
        Parallel::Initialize(host_comm_in);
    }

    void Initialize(int numWallPoints_in)
    {
        memory = GlobalMemoryHandler(numWallPoints_in, settings.rayDim);
    }

    void Finalize(void)
    {
        Parallel::Finalize();
    }
}
