#ifndef BASE_POINTER_TYPES_H
#define BASE_POINTER_TYPES_H
#include <string>
namespace PropTreeLib
{
    namespace Variables
    {
        enum BasePointer
        {
            None,
            IntPointer,
            DoublePointer,
            BoolPointer,
            IntArrayPointer,
            DoubleArrayPointer,
            BoolArrayPointer,
            StringPointer,
            CustomClassPointers
        };
        static inline std::string BasePointerToString(BasePointer p)
        {
            switch (p)
            {
                case BasePointer::None:                return "BasePointer::None";
                case BasePointer::IntPointer:          return "BasePointer::IntPointer";
                case BasePointer::DoublePointer:       return "BasePointer::DoublePointer";
                case BasePointer::BoolPointer:         return "BasePointer::BoolPointer";
                case BasePointer::IntArrayPointer:     return "BasePointer::IntArrayPointer";
                case BasePointer::DoubleArrayPointer:  return "BasePointer::DoubleArrayPointer";
                case BasePointer::BoolArrayPointer:    return "BasePointer::BoolArrayPointer";
                case BasePointer::StringPointer:       return "BasePointer::StringPointer";
                case BasePointer::CustomClassPointers: return "BasePointer::CustomClassPointers";
                default: return "NO_BASE_PTR";
            }
        }
    }
}

#endif
