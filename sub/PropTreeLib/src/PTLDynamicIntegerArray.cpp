#include <iostream>
#include <string>
#include "PTLDynamicIntegerArray.h"
namespace PropTreeLib
{
    namespace Variables
    {
        PTLDynamicIntegerArray::PTLDynamicIntegerArray(std::string descriptionIn)
        {
            strHandle = new PropStringHandler();
            this->SetDescription(descriptionIn);
            defaultValue = "[]";
            basePointerType = BasePointer::IntArrayPointer;
            secondaryBasePointerType=BasePointer::IntPointer;
            requiresDelete = false;
            basePtr = NULL;
            count = 0;
        }
        bool PTLDynamicIntegerArray::ParseFromString(std::string parseVal, void* ptr)
        {
            char open, close;
            strHandle->GetVectorStyle(&open, &close);
            size_t sPos = parseVal.find(open);
            size_t ePos = parseVal.find(close);
            if (ePos-sPos==1) return true; //empty
            if (sPos!=0||ePos!=parseVal.length()-1) return false;
            std::string internalValues = parseVal.substr(sPos+1,ePos-sPos-1);
            std::vector<std::string> vals = strHandle->IdentifyTopLevels(internalValues);
            count = vals.size();
            basePtr = new int[count];
            *((int**)ptr) = basePtr;
            requiresDelete = true;
            std::string nums = "0123456789";
            std::string parseValCurrent;
            for (int j = 0; j < vals.size(); j++)
            {
                int i;
                parseValCurrent = vals[j];
                try
                {
                    for (size_t y = 0; y < parseValCurrent.length(); y++)
                    {
                        if (nums.find(parseValCurrent[y]) == std::string::npos)
                        {
                            this->SetDefaultValue(ptr);
                            return false;
                        }
                    }
                    i=std::stoi(parseValCurrent);
                    *(basePtr+j) = i;
                }
                catch (...)
                {
                    this->SetDefaultValue(ptr);
                    return false;
                }
            }
            return true;
        }
        void PTLDynamicIntegerArray::SetSecondaryVariable(void* ptr)
        {
            *((int*)ptr) = count;
        }
        std::string PTLDynamicIntegerArray::GetDefaultValueString(void)
        {
            return std::string(defaultValue);
        }
        void PTLDynamicIntegerArray::SetDefaultValue(void* ptr){}
        void PTLDynamicIntegerArray::Destroy(void)
        {
            delete strHandle;
            if (requiresDelete) delete [] basePtr;
        }
    }
}
