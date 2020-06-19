#ifndef INPUT_VAR_H
#define INPUT_VAR_H

#include <string>

namespace PropTreeLib
{
    namespace Variables
    {
        class InputVariable
        {
            public:
                virtual bool ParseFromString(std::string parseVal, void* ptr)=0;
                virtual void Destroy(void)=0;
                virtual std::string GetDescription(void) {return variableDescription;}
                virtual void SetDescription(std::string val) {variableDescription = val;}
                virtual void SetDefaultValue(void* ptr)=0;
                virtual std::string GetDefaultValueString(void)=0;
                virtual void SetName(std::string name) {variableName = name;}
            private:
                std::string variableDescription;
                std::string variableName;
        };
    }
}

#endif
