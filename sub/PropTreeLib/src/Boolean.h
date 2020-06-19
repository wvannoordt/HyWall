#ifndef BOOLEAN_H
#define BOOLEAN_H

#include <string>
#include "InputVariable.h"

namespace PropTreeLib
{
    namespace Variables
    {
        class Boolean : public InputVariable
        {
            public:
                Boolean(bool defaultValue, std::string description);
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
