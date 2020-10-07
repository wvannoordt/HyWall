#ifndef DEBUG_TOOLS_H
#define DEBUG_TOOLS_H
#include "HybridComputing.h"
#include <iostream>

#define __longline "---------------------------------------------------------"

#if ___cpu
#define __dump(mystuff) {std::cout << __FILE__ <<  " (" << __LINE__ << "): " << mystuff << std::endl;}
#define __qdump(mystuff) {std::cout << mystuff << std::endl;}
#define __dumpwait(mystuff) {std::cout << __FILE__ <<  " (" << __LINE__ << "): " << mystuff << std::endl;std::cin.get();}
#define __erkill(mystuff) {std::cout << __longline <<std::endl << "Terminate called from " << __FILE__ <<  ", line (" << __LINE__ << "): " << mystuff << std::endl << __longline << std::endl; abort();}
#define __sdump(mystuff) {std::cout << #mystuff << ": " << mystuff << std::endl;}
#endif


#if ___gpu
#define __dump(mystuff) ;
#define __qdump(mystuff) ;
#define __dumpwait(mystuff) ;
#define __erkill(mystuff) ;
#define __sdump(mystuff) ;
#endif

#endif
