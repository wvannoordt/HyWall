#ifndef HYWALL_H
#define HYWALL_H
#include "PropTreeLib.h"
#include "HyWallInstanceSeries.h"
#include "CliInput.h"
#include "ParallelHandler.h"
namespace HyWall
{
    extern PropTreeLib::PropertyTree inputTree;
    extern HyWallInstanceSeries hyWallInstances;
    extern CliInput commandLine;
    extern Parallel::ParallelHandler parallel;
    void Initialize(int argc, char** argv);
    void ReadInputFile(std::string inputFileName);
}

#endif
