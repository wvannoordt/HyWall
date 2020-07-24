#include "Interactive.h"
#include <string>
#include "PropTreeLib.h"
#include "PropertyTree.h"
#include "Error.h"
namespace PropTreeLib
{
    Interactive::Interactive(int argc_in, char** argv_in, PropertyTree* pTree_in)
    {
        argc = argc_in;
        argv = argv_in;
        pTree = pTree_in;
        ptlOptionSpecification = "PTL:";
    }

    void Interactive::Run(void)
    {
        std::string cmd, arg;
        for (int i = 0; i < argc; i++)
        {
            std::string cl(argv[i]);
            ParseArg(cl, &cmd, &arg);
            if (cmd=="defaultVals")
            {
                pTree->CreateDefaultValuesFile(arg);
                EndProgram();
            }
        }
    }

    void Interactive::EndProgram(void)
    {
        ErrorKill("Terminated program after interactive invocation.");
    }

    void Interactive::ParseArg(std::string line, std::string* cmd, std::string* arg)
    {
        *cmd = "";
        *arg = "";
        if (line.find(ptlOptionSpecification) != std::string::npos)
        {
            std::string pval = line.substr(ptlOptionSpecification.length(), line.length()-ptlOptionSpecification.length());
            size_t n = pval.find("=");
            if (n!=std::string::npos)
            {
                *cmd = pval.substr(0, n);
                *arg = pval.substr(n+1, pval.length()-1-n);
            }
            else
            {
                *cmd = pval;
            }
        }
    }
}
