#include <iostream>
#include <string>
#include "PTLAutoEnum.h"
#include <map>
#include <vector>

#define MAX_ENUM_OPTIONS 1024


namespace PropTreeLib
{
    namespace Variables
    {
        PTLAutoEnum::PTLAutoEnum(int defaultValueIn, std::string (*stringifier)(int), std::string descriptionIn)
        {
            std::string acceptable = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_-";
            this->SetDescription(descriptionIn);
            defaultValue = stringifier(defaultValueIn);
            basePointerType = BasePointer::IntPointer;
            for (int i = 0; ((i < MAX_ENUM_OPTIONS)&&(stringifier(i)!=PTL_AUTO_ENUM_TERMINATOR)); i++)
            {
                options.insert({stringifier(i), i});
            }
        }

        std::string PTLAutoEnum::GetAcceptableValueString(void)
        {
            return "";
        }
        bool PTLAutoEnum::ParseFromString(std::string parseVal, void* ptr)
        {
            if (options.find(parseVal)!=options.end())
            {
                *((int*)ptr) = options[parseVal];
                return true;
            }
            else
            {
                this->SetDefaultValue(ptr);
                return false;
            }
        }
        std::string PTLAutoEnum::GetDefaultValueString(void)
        {
            return defaultValue;
        }
        void PTLAutoEnum::SetDefaultValue(void* ptr)
        {
            *((int*)ptr) = options[defaultValue];
        }
        void PTLAutoEnum::Destroy(void)
        {
            options.clear();
        }
    }
}
