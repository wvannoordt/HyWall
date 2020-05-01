#ifndef HYB_COMP_H
#define HYB_COMP_H

#ifndef CUDA_ENABLE
#define CUDA_ENABLE 0
#endif

#ifdef __CUDACC__
#define __commonex(my_extension) template <const int DUMMY, my_extension> __device__
#define __common template <const int DUMMY> __device__
#define __commontemplate template __device__
#define __gpu 1
#define __cpu 0
#define __hybrid 123456
#define __GPUSYNC __syncthreads()
#define CPUONLY(mycode) ;
#define CPUKILL
#define CPUERR(mycode) ;
#define HYBRID HybridGpu
#define __commonvar __device__
#else
#define __commonex(my_extension) template <const int DUMMY, my_extension>
#define __common template <const int DUMMY>
#define __commontemplate template
#define __gpu 0
#define __cpu 1
#define __hybrid 654321
#define __GPUSYNC
#define CPUONLY(mycode) mycode
#define WMLINESTRING "------------------------------------------------------------"
#define CPUKILL std::cout << WMLINESTRING << "\nTermination called in file " << __FILE__ << ", line " << __LINE__ << ".\n" << WMLINESTRING << "\n"; abort();
#define CPUERR(mycode) std::cout << WMLINESTRING << "\nTermination called in file " << __FILE__ << ", line " << __LINE__ << ".\n" << "Message:\n" << mycode << "\n" << WMLINESTRING << std::endl; abort();
#define HYBRID HybridCpu
#define __commonvar
#endif

#endif
