#include <iostream>
#include <string>
#include "PropertyTree.h"
#include "PropStringHandler.h"
namespace PropTreeLib
{
    PropertyTree::PropertyTree(void)
    {
        isInitialized = false;
        stringHandler = PropStringHandler();
    }
    PropertyTree::~PropertyTree(void)
    {
        if (isInitialized)
        {
            principalSection->Destroy();
            delete principalSection;
        }
    }

    void PropertyTree::BuildFromFile(std::string filename)
    {
        std::string fileRawContents = stringHandler.Sanitize(stringHandler.ReadFileToString(filename));
        principalSection = new PropertySection(fileRawContents, &stringHandler, 0);
        isInitialized = true;
    }

    void PropertyTree::DebugPrint(void)
    {
        principalSection->DebugPrint();
    }
}
