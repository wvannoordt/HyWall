#include <iostream>
#include "PropTreeLib.h"
int main(void)
{
	std::string filename = "testfile.in";
	PropTreeLib::PropertyTree input;

	int testInteger1, testInteger2;
	input["userDefinedSection"]["test1"].MapTo(&testInteger1) = new PropTreeLib::Variables::PTLInteger(199, "A test integer");
	input["userDefinedSection"]["test2"].MapTo(&testInteger2) = new PropTreeLib::Variables::PTLInteger(100, "A test integer2");
	input.ReadInputFileToTreeData(filename);
	input.StrictParse();
	std::cout << "Test read the following: " << testInteger1 << ", " << testInteger2 << std::endl;
	std::cout << "(SUCCESS)" << std::endl;
	return 0;
}
