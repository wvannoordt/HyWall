#ifndef SCREEN_OUT_H
#define SCREEN_OUT_H
#include "HyWall.h"
#include <string>
#include <iostream>
#include "Config.hx"

#if(TRACK_OUTPUT_ORIGINS)
#define WRITE_FILE_LINE std::cout << "Output from file " << file << ", line " << line << ":" << std::endl
#else
#define WRITE_FILE_LINE
#endif

#define GET_MACRO(_1,_2,_3,NAME,...) NAME
#define WriteLine(...) GET_MACRO(__VA_ARGS__, WriteLine2, WriteLine1)(__VA_ARGS__)
#define WriteLine1(myLevel, myMessage) internal_output(myLevel, myMessage, __FILE__, __LINE__)
#define WriteLine2(myLevel, myMessage, myStyle) internal_output(myLevel, myMessage, myStyle, __FILE__, __LINE__)

namespace HyWall
{
    void internal_output(int messageVerboseLevel, std::string message, const char* file, const int line);
    void internal_output(int messageVerboseLevel, std::string message, std::string styleString, const char* file, const int line);
}

#endif
