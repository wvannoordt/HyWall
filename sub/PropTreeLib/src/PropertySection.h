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
            PropertySection(std::string contents, PropStringHandler* stringHandler, int depthIn);
            void DebugPrint(void);
            void Destroy(void);
        private:
            std::map<std::string,std::string> sectionValues;
            std::map<std::string,PropertySection*> sectionSubSections;
            int depth;
    };
}
#endif
