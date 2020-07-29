#ifndef HYWALL_INSTANCE_SERIES_H
#define HYWALL_INSTANCE_SERIES_H
#include <string>
#include <vector>
#include "HyWallInstance.h"
namespace HyWall
{
    class HyWallRegistry
    {
        public:
            HyWallRegistry(void);
            ~HyWallRegistry(void);
        private:
            std::vector<HyWallInstance*> instances;
    };
}

#endif
