#ifndef PTLString_H
#define PTLString_H

#include <string>
#include "InputVariable.h"

namespace PropTreeLib
{
    namespace Variables
    {
        class PTLString : public InputVariable
        {
            public:
                PTLString(std::string defaultValue, std::string description);
                bool ParseFromString(std::string parseVal, void* ptr);
                void Destroy(void);
                void SetDefaultValue(void* ptr);
                std::string GetDefaultValueString(void);
            private:
                std::string defaultValue;
        };
    }
}

#endif
