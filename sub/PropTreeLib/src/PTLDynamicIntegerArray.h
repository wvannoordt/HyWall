#ifndef PTLDynamicIntegerArray_H
#define PTLDynamicIntegerArray_H

#include <string>
#include "InputVariable.h"
#include "PropStringHandler.h"

namespace PropTreeLib
{
    namespace Variables
    {
        class PTLDynamicIntegerArray : public InputVariable
        {
            public:
                PTLDynamicIntegerArray(std::string description);
                bool ParseFromString(std::string parseVal, void* ptr);
                void Destroy(void);
                void SetDefaultValue(void* ptr);
                std::string GetDefaultValueString(void);
                void SetSecondaryVariable(void* ptr);
            private:
                const char* defaultValue;
                bool requiresDelete;
                PropStringHandler* strHandle;
                int* basePtr;
                int count;
        };
    }
}

#endif
