#ifndef BUF_FLAGS_H
#define BUF_FLAGS_H

#pragma HWPP include

namespace HyWall
{
    namespace bflag
    {
        enum bflagenum
        {
            input           = 0b00000000000000000000000000000001,
            constInput      = 0b00000000000000000000000000000010,
            output          = 0b00000000000000000000000000000100,
            userMustProvide = 0b00000000000000000000000000001000,
            userCanProvide  = 0b00000000000000000000000000010000,
            solution        = 0b00000000000000000000000000100000,
            cpuOnly         = 0b00000000000000000000000001000000,
            auxilary        = 0b00000000000000000000000010000000
        };
    }
}
#endif
