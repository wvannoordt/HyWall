#ifndef HYB_COMP_H
#define HYB_COMP_H
#include "Config.h"

#pragma HWPP include

#if(CUDA_ENABLE)
#define __withCuda(mycode) {mycode;}
#else
#define __withCuda(mycode) ;
#endif

#ifdef __CUDACC__

#define __common __device__
#define __gpu 1
#define __cpu 0
#define HyCore HyCoreGPU

#else

#define __common
#define __gpu 0
#define __cpu 1
#define HyCore HyCoreCPU

#endif

#endif
