#include "HyWallInstanceSeries.h"
#include "ScreenOutput.h"
#include <string>
#include <vector>
#include "HyWallInstance.h"
#include "DebugTools.hx"
namespace HyWall
{
    HyWallInstanceSeries::HyWallInstanceSeries(void)
    {
        
    }

    HyWallInstanceSeries::~HyWallInstanceSeries(void)
    {
        for (int i = 0; i < instances.size(); i++) {delete instances[i];}
    }
}
