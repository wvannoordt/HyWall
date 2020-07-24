#ifndef INTERACTIVE_H
#define INTERACTIVE_H
#include <string>
#include "PropTreeLib.h"
#include "PropertyTree.h"
namespace PropTreeLib
{
    class Interactive
    {
        public:
            Interactive(int argc_in, char** argv_in, PropertyTree* pTree_in);
            void ParseArg(std::string line, std::string* cmd, std::string* arg);
            void Run(void);
        private:
            void EndProgram(void);
            std::string ptlOptionSpecification;
            PropertyTree* pTree;
            int argc;
            char** argv;
    };
}

#endif
