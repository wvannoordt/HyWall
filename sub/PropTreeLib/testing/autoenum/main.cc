#include <iostream>
#include <string>
#include "PropTreeLib.h"

enum MySpecialEnum
{
	option1,
	option2,
	option3,
	option4
};

std::string MySpecialEnumToString(int i)
{
	switch (i)
	{
		case option1: return "option1";
		case option2: return "option2";
		case option3: return "option3";
		case option4: return "option4";
	}
	return PTL_AUTO_ENUM_TERMINATOR;
}

int main(void)
{
	std::string filename = "testfile.ptl";
	PropTreeLib::PropertyTree input;

	int myval;
	int somevalue;

	input["myIntegerValue"].MapTo(&myval) = new PropTreeLib::Variables::PTLInteger(1, "Some integer value");
	input["myEnumValue"].MapTo(&somevalue) = new PropTreeLib::Variables::PTLAutoEnum(option1, MySpecialEnumToString, "Some enum value");

	input.ReadInputFileToTreeData(filename);
	input.StrictParse();
	input.DebugPrint();
	return 0;
}
