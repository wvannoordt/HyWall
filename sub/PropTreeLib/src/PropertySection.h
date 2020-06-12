#ifndef PROPSECTION_H
#define PROPSECTION_H
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "PropStringHandler.h"
namespace PropTreeLib
{
    class PropertySection
    {
        public:
            PropertySection(PropStringHandler* stringHandler_in, int depthIn, PropertySection* host_in);
            void PopulateInstanceFromString(std::string contents);
            void DebugPrint(void);
            void DeclareIsFromTemplateDeclaration(void);
            void Destroy(void);
            PropertySection& operator [](std::string argument);
        private:
            std::map<std::string,std::string> sectionValues;
            std::map<std::string,PropertySection*> sectionSubSections;
            PropStringHandler* stringHandler;
            int depth;
            bool wasCreatedFromTemplateDeclaration, isDummySection;
            PropertySection* host;
    };
}
#endif
