#ifndef PTLAutoEnum_H
#define PTLAutoEnum_H

#include <string>
#include "InputVariable.h"
#include <map>
#include <vector>
#include "PropStringHandler.h"
#define PTL_AUTO_ENUM_TERMINATOR ""

namespace PropTreeLib
{
    namespace Variables
    {
        class PTLAutoEnum : public InputVariable
        {
            public:
                PTLAutoEnum(int defaultValue, std::string (*stringifier)(int), std::string description);
                bool ParseFromString(std::string parseVal, void* ptr);
                void Destroy(void);
                void SetDefaultValue(void* ptr);
                std::string GetDefaultValueString(void);
                std::string GetAcceptableValueString(void);
            private:
                std::string defaultValue;
                std::map<std::string,int> options;
                PropStringHandler* strHandle;

        };
    }
}

#endif
