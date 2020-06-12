#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "PropertySection.h"
#include "PropStringHandler.h"
#include "ElementClasses.h"
namespace PropTreeLib
{
    PropertySection::PropertySection(PropStringHandler* stringHandler_in, int depthIn, PropertySection* host_in)
    {
        depth = depthIn;
        stringHandler = stringHandler_in;
        wasCreatedFromTemplateDeclaration = false;
        isDummySection = false;
        host = host_in;
    }

    void PropertySection::DeclareIsFromTemplateDeclaration(void)
    {
        wasCreatedFromTemplateDeclaration = true;
    }

    void PropertySection::PopulateInstanceFromString(std::string contents)
    {
        std::vector<std::string> topLevelElements = stringHandler->IdentifyTopLevels(contents);
        for (int i = 0; i < topLevelElements.size(); i++)
        {
            int elementClass = stringHandler->GetElementClass(topLevelElements[i]);
            switch (elementClass)
            {
                case EC_VARASSIGN:
                {
                    std::string name, val;
                    stringHandler->ParseElementAsVariable(topLevelElements[i], &name, &val);
                    sectionValues.insert({name, val});
                    break;
                }
                case EC_SUBSECTION:
                {
                    std::string name, val;
                    stringHandler->ParseElementAsSubSection(topLevelElements[i], &name, &val);
                    sectionSubSections.insert({name, new PropertySection(stringHandler, depth+1, this)});
                    sectionSubSections[name]->PopulateInstanceFromString(val);
                    break;
                }
            }
        }
    }

    void PropertySection::DebugPrint(void)
    {
        std::string style = "|";
        for (int i = 0; i < depth; i++) style = style + "--";
        for (std::map<std::string, std::string>::iterator it = sectionValues.begin(); it!=sectionValues.end(); it++)
        {
            std::cout << style << " " << it->first << " = " << it->second << std::endl;
        }
        for (std::map<std::string, PropertySection*>::iterator it = sectionSubSections.begin(); it!=sectionSubSections.end(); it++)
        {
            std::cout << style << " " << it->first << ":" << std::endl;
            it->second->DebugPrint();
        }

    }

    void PropertySection::Destroy(void)
    {
        for (std::map<std::string, PropertySection*>::iterator it = sectionSubSections.begin(); it!=sectionSubSections.end(); it++)
        {
            it->second->Destroy();
            delete it->second;
        }
    }

    PropertySection& PropertySection::operator [](std::string argument)
    {
        if (sectionSubSections.find(argument)==sectionSubSections.end()) sectionSubSections.insert({argument, new PropertySection(stringHandler, depth+1, this)});
        PropertySection* temp = sectionSubSections[argument];
        temp->DeclareIsFromTemplateDeclaration();
        return *temp;
    }
}
