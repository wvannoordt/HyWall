#include <iostream>
#include "PropTreeLib.h"
namespace PropTreeLib
{
    void ReadInputFileToPropertyTree(std::string filename, PropertyTree* outputPropertyTree)
    {
        outputPropertyTree->BuildFromFile(filename);
    }
}
