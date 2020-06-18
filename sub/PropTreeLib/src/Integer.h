#ifndef INTEGER_H
#define INTEGER_H

#include <string>
#include "InputVariable.h"

namespace PropTreeLib
{
    namespace Variables
    {
        class Integer : public InputVariable
        {
            public:
                Integer(int defaultValue, std::string description);
                bool ParseFromString(std::string parseVal, void* ptr);
                void Destroy(void);
                void SetDefaultValue(void* ptr);
            private:
                int defaultValue;
        };
    }
}

#endif
