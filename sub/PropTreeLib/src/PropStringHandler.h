#ifndef PROPSTRINGHANDLER_H
#define PROPSTRINGHANDLER_H
#include <string>
#include <vector>
namespace PropTreeLib
{
    class PropStringHandler
    {
        public:
            PropStringHandler(void);
            bool fileExists(std::string filename);
            std::string ReadFileToString(std::string filename);
            std::string Sanitize(std::string contents);
            std::string ProcessLine(std::string line);
            size_t GetOriginalLine(size_t compactPosition);
            void AssertGroupingConsistency(std::string str);
            std::string charString(char x);
            std::vector<std::string> IdentifyTopLevels(std::string line);
            void ParseElementAsVariable(std::string line, std::string* variableName, std::string* variableValue);
            void ParseElementAsSubSection(std::string line, std::string* sectionName, std::string* sectionContents);
            int GetElementClass(std::string element);
            void SetDelimiter(char dlm);
        private:
            std::string commentString;
            std::string whiteSpace;
            std::string forbiddenNameChars;
            char openSection;
            char closeSection;
            char openVector;
            char closeVector;
            char assignChar;
            char delimiter;
            std::vector<size_t> lineBreaks;
    };
}

#endif
