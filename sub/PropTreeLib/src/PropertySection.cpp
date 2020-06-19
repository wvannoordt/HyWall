#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "PropertySection.h"
#include "PropStringHandler.h"
#include "ElementClasses.h"
#include "Error.h"
namespace PropTreeLib
{
    PropertySection::PropertySection(PropStringHandler* stringHandler_in, int depthIn, PropertySection* host_in)
    {
        depth = depthIn;
        stringHandler = stringHandler_in;
        wasCreatedFromTemplateDeclaration = false;
        isTerminalNode = false;
        host = host_in;
        templateVariable = NULL;
        terminalEndpointTarget = NULL;
        hasValue = false;
    }

    void PropertySection::DeclareIsFromTemplateDeclaration(void)
    {
        wasCreatedFromTemplateDeclaration = true;
    }

    void PropertySection::DeclareIsTerminal(void)
    {
        isTerminalNode = true;
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
                    if (sectionSubSections.find(name)==sectionSubSections.end())
                    {
                        sectionSubSections.insert({name, new PropertySection(stringHandler, depth+1, this)});
                    }
                    sectionSubSections[name]->DeclareIsTerminal();
                    sectionSubSections[name]->SetName(name);
                    sectionSubSections[name]->SetValue(val);
                    break;
                }
                case EC_SUBSECTION:
                {
                    std::string name, val;
                    stringHandler->ParseElementAsSubSection(topLevelElements[i], &name, &val);
                    if (sectionSubSections.find(name)==sectionSubSections.end())
                    {
                        sectionSubSections.insert({name, new PropertySection(stringHandler, depth+1, this)});
                    }
                    sectionSubSections[name]->PopulateInstanceFromString(val);
                    sectionSubSections[name]->SetName(name);
                    sectionSubSections[name]->SetValue(val);
                    break;
                }
            }
        }
    }

    void PropertySection::SetName(std::string name)
    {
        sectionName = name;
    }

    void PropertySection::SetValue(std::string val)
    {
        sectionValue = val;
        hasValue = true;
    }

    void PropertySection::SetNoValue(void)
    {
        sectionValue = "[DEFAULT]";
        hasValue = false;
    }

    void PropertySection::DebugPrint(void)
    {
        std::string style = "|";
        for (int i = 0; i < depth; i++) style = style + "--";
        if (isTerminalNode)
        {
            std::cout << style << " " << sectionName << " = " << sectionValue << std::endl;
        }
        else
        {
            std::cout << style << " " << sectionName << ":" << std::endl;
        }
        for (std::map<std::string, PropertySection*>::iterator it = sectionSubSections.begin(); it!=sectionSubSections.end(); it++)
        {
            it->second->DebugPrint();
        }

    }

    void PropertySection::Destroy(void)
    {
        if (templateVariable != NULL)
        {
            templateVariable->Destroy();
            delete templateVariable;
        }
        for (std::map<std::string, PropertySection*>::iterator it = sectionSubSections.begin(); it!=sectionSubSections.end(); it++)
        {
            it->second->Destroy();
            PropertySection* temp = it->second;
            delete temp;
        }
        sectionSubSections.clear();
    }

    bool PropertySection::StrictTraverseParse(std::string depthString)
    {
        std::string newDepthString;
        if (depth == 1) newDepthString = sectionName;
        else if (depth>1) newDepthString = depthString + "::" + sectionName;
        else newDepthString = "";
        if (!isTerminalNode)
        {
            if (templateVariable != NULL) ErrorKill("A variable template has been assined to a non-terminal section called \"" + newDepthString + "\"");
            bool anyBranchFailed = false;
            for (std::map<std::string, PropertySection*>::iterator it = sectionSubSections.begin(); it!=sectionSubSections.end(); it++)
            {
                if(!it->second->StrictTraverseParse(newDepthString)) anyBranchFailed = true;
            }
            return !anyBranchFailed;
        }
        else
        {
            if (templateVariable==NULL)
            {
                std::cout << "Unrecognized variable:" << std::endl;
                std::cout << "  >>  " << newDepthString << "  =  " << sectionValue << std::endl;
                return false;
            }
            else if(!hasValue)
            {
                templateVariable->SetDefaultValue(terminalEndpointTarget);
                return true;
            }
            else if(!templateVariable->ParseFromString(sectionValue, terminalEndpointTarget))
            {
                std::cout << "Could not parse the following variable:" << std::endl;
                std::cout << "  >>  " << newDepthString << "  =  " << sectionValue << std::endl;
                return false;
            }
            return true;
        }


    }

    PropertySection& PropertySection::operator [](std::string argument)
    {
        bool isNewSection = false;
        if (sectionSubSections.find(argument)==sectionSubSections.end())
        {
            isNewSection = true;
            sectionSubSections.insert({argument, new PropertySection(stringHandler, depth+1, this)});
        }
        PropertySection* temp = sectionSubSections[argument];
        if (isNewSection)
        {
            temp->SetName(argument);
            temp->SetNoValue();
        }
        temp->DeclareIsFromTemplateDeclaration();
        return *temp;
    }

    PropertySection& PropertySection::operator= (std::string argument)
    {
        this->SetValue(argument);
        this->DeclareIsTerminal();
    }

    Variables::InputVariable* & PropertySection::MapTo(int* ptr)
    {
        isTerminalNode = true;
        terminalEndpointTarget = ptr;
        return templateVariable;
    }

    Variables::InputVariable* & PropertySection::MapTo(double* ptr)
    {
        isTerminalNode = true;
        terminalEndpointTarget = ptr;
        return templateVariable;
    }

    Variables::InputVariable* & PropertySection::MapTo(bool* ptr)
    {
        isTerminalNode = true;
        terminalEndpointTarget = ptr;
        return templateVariable;
    }
}
