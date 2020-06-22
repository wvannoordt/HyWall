#ifndef PTLBoolean_H
#define PTLBoolean_H

#include <string>
#include "InputVariable.h"

namespace PropTreeLib
{
    namespace Variables
    {
        class PTLBoolean : public InputVariable
        {
            public:
                PTLBoolean(bool defaultValue, std::string description);
                bool ParseFromString(std::string parseVal, void* ptr);
                void Destroy(void);
                void SetDefaultValue(void* ptr);
                std::string GetDefaultValueString(void);
            private:
                bool defaultValue;
        };
    }
}

#endif
