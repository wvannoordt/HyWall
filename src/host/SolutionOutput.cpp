#include "SolutionOutput.h"
#include "IO.h"
#include "HyWall.h"
namespace HyWall
{
    void InitializeSolutionOutput(void)
    {
        IO::CreateDirectory(settings.workingDirectory);
    }
}
