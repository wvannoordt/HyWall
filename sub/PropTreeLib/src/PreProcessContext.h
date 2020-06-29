#ifndef PREPROCESSCONTEXT_H
#define PREPROCESSCONTEXT_H
#include <vector>
#include <map>
#include <string>

namespace PropTreeLib
{
    class PreProcessContext
    {
        public:
            PreProcessContext(void);
            void SetStyle(char preProcessIndicator_in, char invocationSymbol_in, char invocationStart_in, char invocationEnd_in);
            void SetHostContext(PreProcessContext* host);
            void Destroy(void);
            bool ParseDefinition(std::string inputLine);
            bool ValidateDefinition(std::string inputLine);
            bool ValidateInvocation(std::string inputLine);
            bool ParseInvocationExpression(std::string inputLine, std::string* outputLine);
            std::string DebugPrint(void);
            bool CheckContextDefinition(std::string input, std::string* output);
        private:
            std::map<std::string, std::string> definitions;
            char preProcessIndicator, invocationSymbol, invocationStart, invocationEnd, dlmChar;
            bool isTopLevelContext;
            PreProcessContext* hostContext;
            std::string defineString, forbiddenNameChars;
    };
}
#endif
