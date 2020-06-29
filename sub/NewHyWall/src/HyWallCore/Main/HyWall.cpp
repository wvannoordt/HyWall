#include "Config.hx"
#include "HyWall.h"
#include "PropTreeLib.h"
#include "ScreenOutput.h"
#include <vector>
#include "HyWallInstanceSeries.h"
#include "CliInput.h"
#include <string>
namespace HyWall
{
    HyWallInstanceSeries hyWallInstances;
    PropTreeLib::PropertyTree inputTree;
    CliInput commandLine;

    void Initialize(int argc, char** argv)
    {
        commandLine = CliInput(argc, argv);
        WriteLine(1, "Initialize HyWall");
    }

    void ReadInputFile(std::string inputFileName)
    {

    }
}
