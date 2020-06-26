#ifndef HYWALL_H
#define HYWALL_H
#include "PropTreeLib.h"
#include "HyWallInstanceSeries.h"
namespace HyWall
{
    extern PropTreeLib::PropertyTree inputTree;
    extern HyWallInstanceSeries hyWallInstances;
    void Initialize(int argc, char** argv);
}

#endif
