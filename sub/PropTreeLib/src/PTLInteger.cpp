#include <iostream>
#include <string>
#include "PTLInteger.h"
namespace PropTreeLib
{
    namespace Variables
    {
        PTLInteger::PTLInteger(int defaultValueIn, std::string descriptionIn)
        {
            this->SetDescription(descriptionIn);
            defaultValue = defaultValueIn;
            basePointerType = BasePointer::IntPointer;
        }
        bool PTLInteger::ParseFromString(std::string parseVal, void* ptr)
        {
            std::string nums = "0123456789";
            int i;
            try
            {
                for (size_t y = 0; y < parseVal.length(); y++)
                {
                    if (nums.find(parseVal[y]) == std::string::npos)
                    {
                        this->SetDefaultValue(ptr);
                        return false;
                    }
                }
                i=std::stoi(parseVal);
                *((int*)ptr) = i;
                return true;
            }
            catch (...)
            {
                this->SetDefaultValue(ptr);
                return false;
            }
            return false;
        }
        std::string PTLInteger::GetDefaultValueString(void)
        {
            return std::to_string(defaultValue);
        }
        void PTLInteger::SetDefaultValue(void* ptr)
        {
            *((int*)ptr) = defaultValue;
        }
        void PTLInteger::Destroy(void){}
    }
}
