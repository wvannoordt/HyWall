#ifndef HYWALL_H
#define HYWALL_H
#include "PropTreeLib.h"
#include "HyWallInstanceSeries.h"
#include "CliInput.h"
namespace HyWall
{
    extern PropTreeLib::PropertyTree inputTree;
    extern HyWallInstanceSeries hyWallInstances;
    extern CliInput commandLine;
    void Initialize(int argc, char** argv);
    void ReadInputFile(std::string inputFileName);
}

#endif
