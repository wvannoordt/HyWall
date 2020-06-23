#include <iostream>
#include <string>
#include "PTLDynamicDoubleArray.h"
namespace PropTreeLib
{
    namespace Variables
    {
        PTLDynamicDoubleArray::PTLDynamicDoubleArray(std::string descriptionIn)
        {
            strHandle = new PropStringHandler();
            this->SetDescription(descriptionIn);
            defaultValue = "[NULL] (dynamically allocated)";
            basePointerType = BasePointer::DoubleArrayPointer;
            secondaryBasePointerType=BasePointer::IntPointer;
            requiresDelete = false;
            basePtr = NULL;
            count = 0;
        }
        bool PTLDynamicDoubleArray::ParseFromString(std::string parseVal, void* ptr)
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
            basePtr = new double[count];
            *((double**)ptr) = basePtr;
            requiresDelete = true;
            for (int i = 0; i < vals.size(); i++)
            {
                double z;
                try
                {
                    z=std::stod(vals[i]);
                    basePtr[i] = z;
                }
                catch (...)
                {
                    return false;
                }
            }
            return true;
        }
        void PTLDynamicDoubleArray::SetSecondaryVariable(void* ptr)
        {
            *((int*)ptr) = count;
        }
        std::string PTLDynamicDoubleArray::GetDefaultValueString(void)
        {
            return std::string(defaultValue);
        }
        void PTLDynamicDoubleArray::SetDefaultValue(void* ptr){}
        void PTLDynamicDoubleArray::Destroy(void)
        {
            delete strHandle;
            if (requiresDelete) delete [] basePtr;
        }
    }
}
