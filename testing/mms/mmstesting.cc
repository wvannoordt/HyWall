#include <iostream>
#include "HyWall.h"
#include "ScreenOutput.h"
int main(void)
{
    HyWall::Testing::InitializeAsDummy(100, 1);
    HyWall::Testing::TestMessage(__FILE__);
    HyWall::Testing::MMSSetup();
    HyWall::Testing::AllocateAsDummy();

    double M, T, E;
    double errorTol = 1e-5;
    HyWall::WriteLine(1, "Running MMS test", "[Testing]");
    HyWall::Testing::RunMMS(&M, &T, &E);
    HyWall::WriteLine(1, "Momentum residual:   " + HyWall::to_estring(M), "[Testing]");
    HyWall::WriteLine(1, "Turbulence residual: " + HyWall::to_estring(T), "[Testing]");
    HyWall::WriteLine(1, "Energy residual:     " + HyWall::to_estring(E), "[Testing]");
    bool success = (M+T+E < 3*errorTol);
    if (success) HyWall::WriteLine(1, "Success", "[Testing]");
    else         HyWall::WriteLine(1, "Failure", "[Testing]");
    return success?0:155;
}
