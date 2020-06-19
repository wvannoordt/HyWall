#include <iostream>
#include <string>
#include "Double.h"
namespace PropTreeLib
{
    namespace Variables
    {
        Double::Double(double defaultValueIn, std::string descriptionIn)
        {
            this->SetDescription(descriptionIn);
            defaultValue = defaultValueIn;
        }
        bool Double::ParseFromString(std::string parseVal, void* ptr)
        {
            double i;
            try
            {
                i=std::stod(parseVal);
                *((double*)ptr) = i;
                return true;
            }
            catch (...)
            {
                this->SetDefaultValue(ptr);
                return false;
            }
            return false;
        }
        std::string Double::GetDefaultValueString(void)
        {
            return std::to_string(defaultValue);
        }
        void Double::SetDefaultValue(void* ptr)
        {
            *((double*)ptr) = defaultValue;
        }
        void Double::Destroy(void){}
    }
}
