#ifndef HYB_COMP_H
#define HYB_COMP_H
#include "Config.hx"

#if(CUDA_ENABLE)
#define __withCuda(mycode) {mycode;}
#else
#define __withCuda(mycode) ;
#endif

#ifdef __CUDACC__

#define __common __device__
#define __gpu 1
#define __cpu 0
#define HyWallHybrid HyWallHybridGPU

#else

#define __common
#define __gpu 0
#define __cpu 1
#define HyWallHybrid HyWallHybridCPU

#endif

#endif
