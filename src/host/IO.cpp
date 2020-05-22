#include <string>
#include "IO.h"
#include <sys/stat.h>
#include "DebugTools.h"

namespace HyWall
{
    namespace IO
    {
        void CreateDirectory(std::string dirname)
        {
            int st = mkdir(dirname.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
        }
    }
}
