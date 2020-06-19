#ifndef DOUBLE_H
#define DOUBLE_H

#include <string>
#include "InputVariable.h"

namespace PropTreeLib
{
    namespace Variables
    {
        class Double : public InputVariable
        {
            public:
                Double(double defaultValue, std::string description);
                bool ParseFromString(std::string parseVal, void* ptr);
                void Destroy(void);
                void SetDefaultValue(void* ptr);
                std::string GetDefaultValueString(void);
            private:
                double defaultValue;
        };
    }
}

#endif
