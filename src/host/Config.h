#ifndef CONFIG_H
#define CONFIG_H


#ifndef CUDA_ENABLE
#define CUDA_ENABLE 0
#endif

#ifndef DEBUG_LEVEL
#define DEBUG_LEVEL 0
#endif

#ifndef ALLOW_DEBUG_EXT
#define ALLOW_DEBUG_EXT 0
#endif

#ifndef CU_BLOCK_SIZE
#define CU_BLOCK_SIZE 16
#endif

#ifndef PARALLEL
#define PARALLEL MPI
#endif

#ifndef MAX_BUFFERS
#define MAX_BUFFERS 1024
#endif

#endif
