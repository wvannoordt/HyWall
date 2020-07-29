#ifndef HYWALL_H
#define HYWALL_H
#include "PropTreeLib.h"
#include "HyWallRegistry.h"
#include "CliInput.h"
#include "ParallelHandler.h"
#include "GlobalSettings.h"
namespace HyWall
{
    extern PropTreeLib::PropertyTree inputTree;
    extern HyWallRegistry registry;
    extern CliInput commandLine;
    extern Parallel::ParallelHandler parallel;
    extern GlobalSettings settings;
    extern bool hasReadInputFile;
    void Initialize(int argc, char** argv);
    void ReadInputFile(std::string inputFileName);
    void SetPropTree(PropTreeLib::PropertyTree hostTree);
}

#endif
