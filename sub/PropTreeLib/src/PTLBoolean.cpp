#include <iostream>
#include <string>
#include "PTLBoolean.h"
namespace PropTreeLib
{
    namespace Variables
    {
        PTLBoolean::PTLBoolean(bool defaultValueIn, std::string descriptionIn)
        {
            this->SetDescription(descriptionIn);
            defaultValue = defaultValueIn;
            basePointerType = BasePointer::BoolPointer;
        }
        bool PTLBoolean::ParseFromString(std::string parseVal, void* ptr)
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
        std::string PTLBoolean::GetDefaultValueString(void)
        {
            return std::to_string(defaultValue);
        }
        void PTLBoolean::SetDefaultValue(void* ptr)
        {
            *((bool*)ptr) = defaultValue;
        }
        void PTLBoolean::Destroy(void){}
    }
}
