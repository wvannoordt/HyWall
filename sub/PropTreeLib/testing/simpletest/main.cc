#include <iostream>
#include "PropTreeLib.h"
int main(void)
{
	std::string filename = "testfile.in";
	PropTreeLib::PropertyTree input;

	int testInteger1, testInteger2;
	input["userDefinedSection"]["test1"].MapTo(&testInteger1) = new PropTreeLib::Variables::Integer(199, "A test integer");
	input["userDefinedSection"]["test2"].MapTo(&testInteger2) = new PropTreeLib::Variables::Integer(100, "A test integer2");

	input.ReadInputFileToTreeData(filename);
	//input.DebugPrint();
	//input.StrictParse();

	//std::cout << testInteger1 << ", " << testInteger2 << std::endl;

	//input.DebugPrint();
	/*proto["NavierStokes"]["WallModel"]["rayDim"] = inputVariableTypes.Integer("Number of ray points for wall model mesh");
	proto["NavierStokes"]["WallModel"]["useWallModel"] = inputVariableTypes.Boolean("Flag to turn on wall model");
	proto["NavierStokes"]["WallModel"]["GpuSettings"]["allocationRatio"]  = inputVariableTypes.DoubleBetween(0.0, 1.0, "Allocation ration for gpu settings", default = 0.1);
	proto["NavierStokes"]["WallModel"]["GpuSettings"]["convectiveScheme"] = inputVariableTypes.AnyOf("weno5fvs, description=WENO scheme, 5th order", "centralEEC", "Convective scheme for interior domain");*/
	return 0;
}
