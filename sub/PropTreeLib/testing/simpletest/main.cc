#include <iostream>
#include "PropTreeLib.h"
int main(void)
{
	std::string filename = "testfile.in";
	PropTreeLib::PropertyTree inputParams;
	PropTreeLib::ReadInputFileToPropertyTree(filename, &inputParams);
	inputParams.DebugPrint();
	return 0;
}
