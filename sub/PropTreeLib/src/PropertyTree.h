#ifndef PROPTREE_H
#define PROPTREE_H
#include <iostream>
#include <string>
#include "PropertySection.h"
#include "PropStringHandler.h"

#define TR_PROTOTYPE 123873
#define TR_DATA      232342

namespace PropTreeLib
{
    class PropertyTree
    {
        public:
            PropertyTree(void);
            ~PropertyTree(void);
            void ReadInputFileToTreeData(std::string filename);
            void SetCloseMessage(std::string message);
            void DebugPrint(void);
            PropertySection& operator [](std::string argument);
            void StrictParse(void);
            void Destroy(void);
        private:
            PropertySection* principalSection;
            PropStringHandler stringHandler;
            int treeSpecification;
            std::string closeMessage;
    };
}
#endif
