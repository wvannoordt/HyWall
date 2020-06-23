#ifndef PTLInteger_H
#define PTLInteger_H

#include <string>
#include "InputVariable.h"

namespace PropTreeLib
{
    namespace Variables
    {
        class PTLInteger : public InputVariable
        {
            public:
                PTLInteger(int defaultValueIn, std::string description);
                bool ParseFromString(std::string parseVal, void* ptr);
                void Destroy(void);
                void SetDefaultValue(void* ptr);
                std::string GetDefaultValueString(void);
            private:
                int defaultValue;
        };
    }
}

#endif
