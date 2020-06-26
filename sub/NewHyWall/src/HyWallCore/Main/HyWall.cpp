#include "Config.hx"
#include "HyWall.h"
#include "PropTreeLib.h"
#include "ScreenOutput.h"
#include <vector>
#include "HyWallInstanceSeries.h"
namespace HyWall
{
    HyWallInstanceSeries hyWallInstances;
    PropTreeLib::PropertyTree inputTree;
    void Initialize(int argc, char** argv)
    {
        inputTree.SetCloseMessage("PTL close");
        WriteLine(1, "Initialize HyWall");
    }
}
