#include <iostream>
#include <string>
#include "Integer.h"
namespace PropTreeLib
{
    namespace Variables
    {
        Integer::Integer(int defaultValueIn, std::string descriptionIn)
        {
            this->SetDescription(descriptionIn);
            defaultValue = defaultValueIn;
        }
        bool Integer::ParseFromString(std::string parseVal, void* ptr)
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
        void Integer::SetDefaultValue(void* ptr)
        {
            *((int*)ptr) = defaultValue;
        }
        void Integer::Destroy(void){}
    }
}
