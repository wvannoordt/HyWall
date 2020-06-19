#include <iostream>
#include <string>
#include "Boolean.h"
namespace PropTreeLib
{
    namespace Variables
    {
        Boolean::Boolean(bool defaultValueIn, std::string descriptionIn)
        {
            this->SetDescription(descriptionIn);
            defaultValue = defaultValueIn;
        }
        bool Boolean::ParseFromString(std::string parseVal, void* ptr)
        {
            if ((parseVal=="True")||(parseVal=="true")||(parseVal=="T")||(parseVal=="t"))
            {
                *((bool*)ptr) = true;
                return true;
            }
            if ((parseVal=="False")||(parseVal=="false")||(parseVal=="F")||(parseVal=="f"))
            {
                *((bool*)ptr) = false;
                return true;
            }
            this->SetDefaultValue(ptr);
            return false;
        }
        std::string Boolean::GetDefaultValueString(void)
        {
            return std::to_string(defaultValue);
        }
        void Boolean::SetDefaultValue(void* ptr)
        {
            *((bool*)ptr) = defaultValue;
        }
        void Boolean::Destroy(void){}
    }
}
