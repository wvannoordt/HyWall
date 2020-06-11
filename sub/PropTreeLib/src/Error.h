#ifndef ERROR_H
#define ERROR_H
#include <iostream>
#include <string>
#include <cstdlib>
#define ErrorKill(mycode) {M_ErrorKill(mycode, __LINE__, __FILE__);}
static inline void M_ErrorKill(std::string message, const int line, const char* file)
{
    std::cout << "---------------------------------------" << std::endl;
    std::cout << "Terminate called from file " << file << ", line " << line << ":" << std::endl;
    std::cout << "Message: " << message << std::endl;
    std::cout << "---------------------------------------" << std::endl;
    exit(199);
}

#endif
