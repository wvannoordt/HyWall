#ifndef PTLStaticIntegerArray_H
#define PTLStaticIntegerArray_H

#include <string>
#include "InputVariable.h"
#include "PropStringHandler.h"

namespace PropTreeLib
{
    namespace Variables
    {
        class PTLStaticIntegerArray : public InputVariable
        {
            public:
                PTLStaticIntegerArray(const int assertCount_in, std::string descriptionIn);
                bool ParseFromString(std::string parseVal, void* ptr);
                void Destroy(void);
                void SetDefaultValue(void* ptr);
                std::string GetDefaultValueString(void);
            private:
                const char* defaultValue;
                bool requiresDelete;
                PropStringHandler* strHandle;
                int* basePtr;
                int count;
                int assertCount;
        };
    }
}

#endif
