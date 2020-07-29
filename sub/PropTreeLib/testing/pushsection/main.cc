#include <iostream>
#include "PropTreeLib.h"
int main(void)
{
	std::string filename = "push.ptl";
	PropTreeLib::PropertyTree input;

	int num1, num2, num3;
	input["MainSection"]["value1"].MapTo(&num1) = new PropTreeLib::Variables::PTLInteger(2, "some variable");
	input["MainSection"]["value2"].MapTo(&num2) = new PropTreeLib::Variables::PTLInteger(2, "some variable");
	input["MainSection"].PushSection("PushedSection");
	input["MainSection"]["value3"].MapTo(&num3) = new PropTreeLib::Variables::PTLInteger(2, "some variable");
	
	input.ReadInputFileToTreeData(filename);
	input.StrictParse();
	input.DebugPrint();

	std::cout << "(SUCCESS)" << std::endl;
	return 0;
}
