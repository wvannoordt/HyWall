#include "HyWallCuda.h"
#include "HyWall.h"
#include "GlobalMemoryHandler.h"
#include "HybridComputing.h"
#include "CoreData.h"
#include "UserSettings.h"
#include "Solver.h"
namespace HyWall
{
    __global__ void K_set_settings(UserSettings settings_in)
    {
        HyCore::settings = settings_in;
    }

    __global__ void K_set_access_pitch(int ap)
    {
        HyCore::majorAccessPitch = ap;
    }

    __global__ void K_meta(void)
    {
        HyCore::MetaDataSet(&HyCore::settings);
    }

    void CopyCudaSymbols(void)
    {

        K_set_settings<<<1,1>>>(settings);
        K_set_access_pitch<<<1,1>>>(memory.localGpuPoints);
        K_meta<<<1,1>>>();
        //TODO: calls to cudaMemcpyToSymbol for global pointers!!!!!
    }

    void ComputeGpuSolution(void)
    {
        std::cout << "TERMINATED from file " << __FILE__ << ", line " << __LINE__ << ": no cuda support yet." << std::endl;
        abort();
    }

    void InitGpuSolution(void)
    {
        std::cout << "TERMINATED from file " << __FILE__ << ", line " << __LINE__ << ": no cuda support yet." << std::endl;
        abort();
    }

    void BeginGpuSolution(void)
    {
        std::cout << "TERMINATED from file " << __FILE__ << ", line " << __LINE__ << ": no cuda support yet." << std::endl;
        abort();
    }
}
