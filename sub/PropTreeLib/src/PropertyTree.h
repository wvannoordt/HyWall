#ifndef PROPTREE_H
#define PROPTREE_H
#include <iostream>
#include <string>
#include "PropertySection.h"
#include "PropStringHandler.h"
namespace PropTreeLib
{
    class PropertyTree
    {
        public:
            PropertyTree(void);
            ~PropertyTree(void);
            void BuildFromFile(std::string filename);
            void DebugPrint(void);
        private:
            PropertySection* principalSection;
            bool isInitialized;
            PropStringHandler stringHandler;
    };
}
#endif
