#ifndef HYWALL_INSTANCE_SERIES_H
#define HYWALL_INSTANCE_SERIES_H
#include <string>
#include <vector>
#include "HyWallInstance.h"
namespace HyWall
{
    class HyWallInstanceSeries
    {
        public:
            HyWallInstanceSeries(void);
            ~HyWallInstanceSeries(void);
        private:
            std::string instanceName;
            std::vector<HyWallInstance*> instances;
    };
}

#endif
