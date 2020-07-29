#include <iostream>
#include "HyWall.h"
int main(int argc, char** argv)
{
    HyWall::Initialize(argc, argv);
    HyWall::ReadInputFile("hywall.ptl");
    return 0;
}
