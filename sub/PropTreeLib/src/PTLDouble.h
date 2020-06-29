#ifndef PTLDouble_H
#define PTLDouble_H

#include <string>
#include "InputVariable.h"

namespace PropTreeLib
{
    namespace Variables
    {
        class PTLDouble : public InputVariable
        {
            public:
                PTLDouble(double defaultValue, std::string description);
                bool ParseFromString(std::string parseVal, void* ptr);
                void Destroy(void);
                void SetDefaultValue(void* ptr);
                std::string GetDefaultValueString(void);
            private:
                double defaultValue;
                std::string allowed;
        };
    }
}

#endif
