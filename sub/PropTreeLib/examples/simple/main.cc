#include <iostream>
#include "PropTreeLib.h"
int main(void)
{
    //This is the simplest example of a PTL input.

    //Define an input class
    PropTreeLib::PropertyTree input;

    //Target variable to read data into
    int testValue;

    //Define a template variable and map to target
    input["testValue"].MapTo(&testValue) = new PropTreeLib::Variables::PTLInteger(1, "A test value with default value 1");

    //Read the input file
    input.ReadInputFileToTreeData("test.ptl");

    //Parse the input data
    input.StrictParse();

    //Leave happy
    std::cout << "testValue: " << testValue << std::endl;
    return 0;
}
