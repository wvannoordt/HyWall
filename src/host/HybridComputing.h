#ifndef HYB_COMP_H
#define HYB_COMP_H

#pragma HWPP include

#ifdef __CUDACC__
#define __common __device__
#define __gpu 1
#define __cpu 0
#define CPUONLY(mycode) ;
#define CPUKILL
#define CPUERR(mycode) ;
#define HyCore HyCoreGPU
#else
#define __common
#define __gpu 0
#define __cpu 1
#define CPUONLY(mycode) mycode
#define WMLINESTRING "------------------------------------------------------------"
#define CPUKILL {std::cout << WMLINESTRING << "\nTermination called in file " << __FILE__ << ", line " << __LINE__ << ".\n" << WMLINESTRING << "\n"; abort();}
#define CPUERR(mycode) {std::cout << WMLINESTRING << "\nTermination called in file " << __FILE__ << ", line " << __LINE__ << ".\n" << "Message:\n" << mycode << "\n" << WMLINESTRING << std::endl; abort();}
#define HyCore HyCoreCPU
#endif

#endif
