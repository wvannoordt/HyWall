#include <iostream>
#include <string>
#include "PTLStaticDoubleArray.h"
namespace PropTreeLib
{
    namespace Variables
    {
        PTLStaticDoubleArray::PTLStaticDoubleArray(int assertCount_in, std::string descriptionIn)
        {
            strHandle = new PropStringHandler();
            this->SetDescription(descriptionIn);
            defaultValue = "[]";
            basePointerType = BasePointer::DoubleArrayPointer;
            requiresDelete = false;
            basePtr = NULL;
            count = 0;
            assertCount = assertCount_in;
        }
        bool PTLStaticDoubleArray::ParseFromString(std::string parseVal, void* ptr)
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
            if (count!=assertCount) return false;
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
        std::string PTLStaticDoubleArray::GetDefaultValueString(void)
        {
            return std::string(defaultValue);
        }
        void PTLStaticDoubleArray::SetDefaultValue(void* ptr){}
        void PTLStaticDoubleArray::Destroy(void)
        {
            delete strHandle;
            if (requiresDelete) delete [] basePtr;
        }
    }
}
