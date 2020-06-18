#include <iostream>
#include <string>
#include "Error.h"
#include "PropertyTree.h"
#include "PropStringHandler.h"
namespace PropTreeLib
{
    PropertyTree::PropertyTree(void)
    {
        stringHandler = PropStringHandler();
        principalSection = new PropertySection(&stringHandler, 0, NULL);
    }

    PropertyTree::~PropertyTree(void)
    {
        principalSection->Destroy();
        delete principalSection;
    }

    void PropertyTree::ReadInputFileToTreeData(std::string filename)
    {
        std::string fileRawContents = stringHandler.Sanitize(stringHandler.ReadFileToString(filename));
        principalSection->PopulateInstanceFromString(fileRawContents);
        principalSection->SetName("[ROOT]");
        principalSection->SetValue(fileRawContents);
    }

    void PropertyTree::DebugPrint(void)
    {
        principalSection->DebugPrint();
    }

    void PropertyTree::StrictParse(void)
    {
        if (!principalSection->StrictTraverseParse(""))
        {
            ErrorKill("Found at least 1 invalid argument. Stopping.");
        }
    }

    PropertySection& PropertyTree::operator [](std::string argument)
    {
        return (*principalSection)[argument];
    }
}
