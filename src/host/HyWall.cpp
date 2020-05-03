#include "HyWall.h"
#include "UserSettings.h"
#include "AllocationModes.h"
#include "TransitionSensors.h"
#include <string>
#include "HWPP.hpp"
namespace HyWall
{
    UserSettings settings;
    GlobalMemoryHandler memory;
    void Initialize(MPI_Comm host_comm_in, int verboseLevel_in)
    {
        HWPP_SetDefaultValues(&settings);
        memory = GlobalMemoryHandler();
        Parallel::Initialize(host_comm_in);
    }

    void SetDomainSize(int numWallPoints_in)
    {
        memory.SetSize(numWallPoints_in, settings.rayDim);
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

    void PassVariable(std::string strname, double* ptr)
    {
        memory.SetUserAssociatedVariable(strname, ptr);
    }

    void Finalize(void)
    {
        Parallel::Finalize();
    }
}
