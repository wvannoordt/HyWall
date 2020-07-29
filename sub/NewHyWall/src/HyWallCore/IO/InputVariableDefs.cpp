#include "HyWall.h"
#include "InputVariableDefs.h"
namespace HyWall
{
    void DefineVariables(void)
    {
        inputTree["verboseLevel"].MapTo(&(settings.verboseLevel))
            = new PropTreeLib::Variables::PTLInteger(1, "Adjusts level of terminal output: 1-4, higher corresponds to more output.");
    }
}
