#include "PreProcessContext.h"
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include "Error.h"

namespace PropTreeLib
{
    PreProcessContext::PreProcessContext(void)
    {
        preProcessIndicator = '#';
        invocationSymbol = '$';
        invocationStart = '(';
        invocationEnd = ')';
        hostContext = NULL;
        isTopLevelContext = true;
        defineString = "define";
        forbiddenNameChars = "!@#$%^&*(){}[]\\|~\'`?/<>,";
        dlmChar = ' ';
    }

    void PreProcessContext::SetHostContext(PreProcessContext* host)
    {
        hostContext = host;
        isTopLevelContext = false;
    }

    void PreProcessContext::SetStyle(char preProcessIndicator_in, char invocationSymbol_in, char invocationStart_in, char invocationEnd_in)
    {
        preProcessIndicator = preProcessIndicator_in;
        invocationSymbol = invocationSymbol_in;
        invocationStart = invocationStart_in;
        invocationEnd = invocationEnd_in;
    }

    void PreProcessContext::Destroy(void)
    {

    }

    bool PreProcessContext::ValidateDefinition(std::string inputLine)
    {
        if (inputLine.find(preProcessIndicator)==0)
        {
            return true;
        }
        return false;
    }

    bool PreProcessContext::ValidateInvocation(std::string inputLine)
    {
        return (inputLine.find(invocationSymbol)!=std::string::npos)&&(inputLine.find(invocationStart)!=std::string::npos)&&(inputLine.find(invocationEnd)!=std::string::npos);
    }

    bool PreProcessContext::ParseDefinition(std::string inputLine)
    {
        bool error = false;
        std::string errorReason = "none";
        std::string defString = preProcessIndicator + defineString + dlmChar;
        size_t pos = inputLine.find(defString);
        if (pos!=0)
        {
            error = true;
            errorReason = "improper preprocessor formatting";
        }
        std::string valassign = inputLine.substr(defString.length(), inputLine.length()-defString.length());
        pos = valassign.find(dlmChar);
        if (pos==std::string::npos)
        {
            error = true;
            errorReason = "could not find value delimiter";
        }
        std::string varName = valassign.substr(0, pos);
        std::string varVal  = valassign.substr(pos+1,valassign.length()-pos-1);
        if (varName.find_first_of(forbiddenNameChars)!=std::string::npos)
        {
            error = true;
            errorReason = "forbidden character";
        }
        if (varName.length()==0)
        {
            error = true;
            errorReason = "empty variable name";
        }
        if (definitions.find(varName)!=definitions.end())
        {
            error = true;
            errorReason = "multiple definition";
        }
        if (error) ErrorKill("Error parsing preprocessor directive \"" + inputLine + "\": " + errorReason + ".");
        definitions.insert({varName,varVal});
        return true;
    }

    bool PreProcessContext::ParseInvocationExpression(std::string inputLine, std::string* outputLine)
    {
        std::vector<size_t> invocations;
        std::vector<size_t> starts;
        std::vector<size_t> ends;
        for (size_t i = 0; i < inputLine.length(); i++)
        {
            if (inputLine[i]==invocationSymbol) invocations.push_back(i);
            if (inputLine[i]==invocationStart) starts.push_back(i);
            if (inputLine[i]==invocationEnd) ends.push_back(i);
        }
        if ((starts.size() != ends.size()) && (invocations.size() != ends.size())) ErrorKill("Error parsing preprocessor invocation \"" + inputLine + "\": unbalanced parentheses or invocations.");
        if (invocations.size()==0) {*outputLine = inputLine;return true;}
        std::vector<std::string> macros;
        for (size_t i = 0; i < starts.size(); i++)
        {
            std::string keyValue = inputLine.substr(starts[i]+1, ends[i]-starts[i]-1);
            std::string macroValue;
            if (!CheckContextDefinition(keyValue, &macroValue)) ErrorKill("Error parsing preprocessor invocation \"" + inputLine + "\": found no suitable definition of keyValue \"" + keyValue + "\".");
            macros.push_back(macroValue);
        }
        std::vector<std::string> baseStrings;
        baseStrings.push_back(inputLine.substr(0, starts[0]-1));
        for (int i = 0; i < ends.size()-1; i++)
        {
            baseStrings.push_back(inputLine.substr(ends[i]+1, starts[i+1]-ends[i]-2));
        }
        baseStrings.push_back(inputLine.substr(ends[ends.size()-1]+1, inputLine.length() - ends[ends.size()-1]-1));
        std::string output = baseStrings[0];
        for (int i = 0; i < macros.size(); i++)
        {
            output = output + macros[i] + baseStrings[i+1];
        }
        *outputLine = output;
        return true;
    }

    bool PreProcessContext::CheckContextDefinition(std::string input, std::string* output)
    {
        if (definitions.find(input)!=definitions.end())
        {
            *output = definitions[input];
            return true;
        }
        if (isTopLevelContext) return false;
        return hostContext->CheckContextDefinition(input, output);
    }

    std::string PreProcessContext::DebugPrint(void)
    {
        std::string output = "";
        if (definitions.size()==0) return output;
        output = output+"{";
        int i = 0;
        for (std::map<std::string, std::string>::iterator it = definitions.begin(); it!=definitions.end(); it++)
        {
            if (i!=0) output = output + ",";
            output += (it->first + "=" + it->second);
            i++;
        }
        return output+"}";
    }
}
