#ifndef SCREEN_OUT_H
#define SCREEN_OUT_H
#include "HyWall.h"
#include <string>
#include <iostream>

namespace HyWall
{
    void WriteLine(int messageVerboseLevel, std::string message);
    void WriteLine(int messageVerboseLevel, std::string message, std::string styleString);
}

#endif
