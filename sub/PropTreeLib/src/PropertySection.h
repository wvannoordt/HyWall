#ifndef PROPSECTION_H
#define PROPSECTION_H
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "PropStringHandler.h"
#include "InputVariable.h"
namespace PropTreeLib
{
    class PropertySection
    {
        public:
            PropertySection(PropStringHandler* stringHandler_in, int depthIn, PropertySection* host_in);
            void PopulateInstanceFromString(std::string contents);
            void SetName(std::string name);
            void SetValue(std::string val);
            void SetNoValue(void);
            void DebugPrint(void);
            void DeclareIsFromTemplateDeclaration(void);
            void DeclareIsTerminal(void);
            void Destroy(void);
            PropertySection& operator [](std::string argument);
            PropertySection& operator =(std::string argument);
            Variables::InputVariable* & MapTo(int* ptr);
            bool StrictTraverseParse(std::string depthString);
        private:
            std::map<std::string,PropertySection*> sectionSubSections;
            PropStringHandler* stringHandler;
            int depth;
            bool wasCreatedFromTemplateDeclaration, isTerminalNode, hasValue;
            PropertySection* host;
            std::string sectionName, sectionValue;
            Variables::InputVariable* templateVariable;
            void* terminalEndpointTarget;
    };
}
#endif
