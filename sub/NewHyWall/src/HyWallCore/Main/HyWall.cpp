#include "Config.hx"
#include "HyWall.h"
#include "PropTreeLib.h"
#include "ScreenOutput.h"
#include <vector>
#include "HyWallRegistry.h"
#include "CliInput.h"
#include "ParallelHandler.h"
#include <string>
#include "DebugTools.hx"
#include "GlobalSettings.h"
#include "InputVariableDefs.h"

namespace HyWall
{
    HyWallRegistry registry;
    PropTreeLib::PropertyTree inputTree;
    CliInput commandLine;
    Parallel::ParallelHandler parallel;
    GlobalSettings settings;
    
    std::string globName;
    bool inputTreeHasHost;

    void Initialize(int argc, char** argv)
    {
        inputTreeHasHost = false;
        commandLine = CliInput(argc, argv);
        globName = "HyWall";
        WriteLine(1, "Initialize HyWall");
    }
    
    void SetPropTree(PropTreeLib::PropertyTree hostTree)
    {
        inputTreeHasHost = true;
        inputTree.SetAsSubtree(hostTree[globName]);
        DefineVariables();
        inputTree.StrictParse();
    }

    void ReadInputFile(std::string inputFileName)
    {
        if (!inputTreeHasHost)
        {
            inputTree.ReadInputFileToTreeData(inputFileName);
            inputTree.PushSection(globName);
            DefineVariables();
            inputTree.StrictParse();
        }
        else
        {
            __erkill("ReadInputFile file called, but subtree was declared.");
        }
    }
}
