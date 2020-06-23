#ifndef PTLEnum_H
#define PTLEnum_H

#include <string>
#include "InputVariable.h"
#include <map>
#include <vector>
#include "PropStringHandler.h"

namespace PropTreeLib
{
    namespace Variables
    {
        class PTLEnum : public InputVariable
        {
            public:
                PTLEnum(std::string defaultValue, std::string formattedOptions, std::string description);
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
