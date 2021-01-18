#include "LoadBalancer.h"
#include "GlobalMemoryHandler.h"
#include "Parallel.h"
namespace HyWall
{
    LoadBalancer::LoadBalancer(GlobalMemoryHandler* mem_in)
    {
        mem = mem_in;
    }
}