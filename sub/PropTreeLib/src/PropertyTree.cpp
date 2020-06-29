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
        principalSection->DeclareIsPrincipal();
        closeMessage = "none";
    }

    PropertyTree::~PropertyTree(void)
    {
        this->Destroy();
    }

    void PropertyTree::Destroy(void)
    {
    	if (closeMessage != "none") std::cout << closeMessage << std::endl;
        principalSection->Destroy();
        delete principalSection;
    }

    void PropertyTree::SetCloseMessage(std::string message)
    {
    	closeMessage = message;
    }

    void PropertyTree::ReadInputFileToTreeData(std::string filename)
    {
        std::string fileRawContents = stringHandler.Sanitize(stringHandler.ReadFileToString(filename), principalSection->GetContext());
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
