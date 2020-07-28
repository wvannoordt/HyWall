#include <iostream>
#include "PropTreeLib.h"
int main(int argc, char** argv)
{
    //This is the simplest example of a PTL input.

    //Define an input class
    PropTreeLib::PropertyTree input;
    PropTreeLib::Interactive i(argc, argv, &input);

    std::string str;
    int aa, ab, ba, bb;
    double* ar;
    int n;

    input["SectionA"]["ValueA"].MapTo(&aa) = new PropTreeLib::Variables::PTLInteger(1, "aa");
    input["SectionA"]["ValueB"].MapTo(&ab) = new PropTreeLib::Variables::PTLInteger(1, "ab");
    input["SectionA"]["StringValue"].MapTo(&str) = new PropTreeLib::Variables::PTLString("defaultVal", "str");
    input["SectionB"]["ValueA"].MapTo(&ba) = new PropTreeLib::Variables::PTLInteger(1, "ba");
    input["SectionB"]["ValueB"].MapTo(&bb) = new PropTreeLib::Variables::PTLInteger(1, "bb");
    input["SectionB"]["arr"].MapTo(&ar, &n) = new PropTreeLib::Variables::PTLDynamicDoubleArray("array");
    i.Run();

    return 0;
}
