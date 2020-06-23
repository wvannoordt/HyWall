#include <iostream>
#include <string>
#include "PTLEnum.h"
#include <map>
#include <vector>
namespace PropTreeLib
{
    namespace Variables
    {
        PTLEnum::PTLEnum(std::string defaultValueIn, std::string formattedOptions, std::string descriptionIn)
        {
            strHandle = new PropStringHandler();
            std::string acceptable = "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ_-";
            if (formattedOptions.length()==0) ErrorKill("PTLEnum declaration with default value \"" + defaultValueIn + "\" has empty option specifier.");
            this->SetDescription(descriptionIn);
            defaultValue = defaultValueIn;
            basePointerType = BasePointer::IntPointer;
            strHandle->SetDelimiter(':');
            std::vector<std::string> optStrings = strHandle->IdentifyTopLevels(formattedOptions);
            for (int i = 0; i < optStrings.size(); i++)
            {
                for (int j = 0; j < optStrings[i].length(); j++)
                {
                    if (acceptable.find(optStrings[i][j])==std::string::npos) ErrorKill("Illegal character \'" + strHandle->charString(optStrings[i][j]) + "\' in PTLEnum declaration \"" + formattedOptions + "\"");
                }
                options.insert({optStrings[i], i});
            }
            int dummy;
            if (!this->ParseFromString(defaultValueIn, &dummy))
            {
                ErrorKill("Default option \"" + defaultValueIn + "\" not found in PTLEnum declaration \"" + formattedOptions + "\"");
            }
        }

        std::string PTLEnum::GetAcceptableValueString(void)
        {
            return "";
        }
        bool PTLEnum::ParseFromString(std::string parseVal, void* ptr)
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
        std::string PTLEnum::GetDefaultValueString(void)
        {
            return defaultValue;
        }
        void PTLEnum::SetDefaultValue(void* ptr)
        {
            *((int*)ptr) = options[defaultValue];
            *((int*)ptr) = 0;
        }
        void PTLEnum::Destroy(void)
        {
            delete strHandle;
            options.clear();
        }
    }
}
