#ifndef PTLDynamicDoubleArray_H
#define PTLDynamicDoubleArray_H

#include <string>
#include "InputVariable.h"
#include "PropStringHandler.h"

namespace PropTreeLib
{
    namespace Variables
    {
        class PTLDynamicDoubleArray : public InputVariable
        {
            public:
                PTLDynamicDoubleArray(std::string description);
                bool ParseFromString(std::string parseVal, void* ptr);
                void Destroy(void);
                void SetDefaultValue(void* ptr);
                std::string GetDefaultValueString(void);
                void SetSecondaryVariable(void* ptr);
            private:
                const char* defaultValue;
                bool requiresDelete;
                PropStringHandler* strHandle;
                double* basePtr;
                int count;
        };
    }
}

#endif
