#ifndef DEBUG_TOOLS_H
#define DEBUG_TOOLS_H
#include "HybridComputing.h"

#define __longline "---------------------------------------------------------"

#if __cpu
#define __dump(mystuff) {std::cout << __FILE__ <<  " (" << __LINE__ << "): " << mystuff << std::endl;}
#define __dumpwait(mystuff) {std::cout << __FILE__ <<  " (" << __LINE__ << "): " << mystuff << std::endl;std::cin.get();}
#define __erkill(mystuff) {std::cout << __longline <<std::endl << "Terminate called from " << __FILE__ <<  ", line (" << __LINE__ << "): " << mystuff << std::endl << __longline << std::endl; abort();}
#endif


#if __gpu
#define __dump(mystuff) ;
#define __dumpwait(mystuff) ;
#define __erkill(mystuff) ;
#endif

#endif
