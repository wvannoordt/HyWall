#ifndef IO_H
#define IO_H

#include <string>

namespace HyWall
{
    namespace IO
    {
        void CreateDirectory(std::string filename);
        void WriteStateByFlags(std::string filename, int flags);
        void ReadState(std::string filename);
        bool FileExists(const std::string& name);
    }
}

#endif
