#include "HyWallRegistry.h"
#include "ScreenOutput.h"
#include <string>
#include <vector>
#include "HyWallInstance.h"
#include "DebugTools.hx"
#include "ParallelHandler.h"
namespace HyWall
{
    HyWallRegistry::HyWallRegistry(void)
    {
        
    }

    HyWallRegistry::~HyWallRegistry(void)
    {
        for (int i = 0; i < instances.size(); i++) {delete instances[i];}
    }
}
