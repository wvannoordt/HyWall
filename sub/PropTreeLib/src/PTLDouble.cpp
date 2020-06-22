#include <iostream>
#include <string>
#include "PTLDouble.h"
namespace PropTreeLib
{
    namespace Variables
    {
        PTLDouble::PTLDouble(double defaultValueIn, std::string descriptionIn)
        {
            this->SetDescription(descriptionIn);
            defaultValue = defaultValueIn;
            basePointerType = BasePointer::DoublePointer;
        }
        bool PTLDouble::ParseFromString(std::string parseVal, void* ptr)
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
        std::string PTLDouble::GetDefaultValueString(void)
        {
            return std::to_string(defaultValue);
        }
        void PTLDouble::SetDefaultValue(void* ptr)
        {
            *((double*)ptr) = defaultValue;
        }
        void PTLDouble::Destroy(void){}
    }
}
