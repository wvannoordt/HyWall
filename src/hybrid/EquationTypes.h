#ifndef EQ_TYPES_H
#define EQ_TYPES_H

#pragma HWPP include

namespace HyCore
{
    namespace momentum
    {
        enum momentumenum
        {
            allmaras         = 34534,
            ODE              = 26323
        };
    }

    namespace turbulence
    {
        enum turbulenceenum
        {
            linear           = 29823,
            ODE              = 23414
        };
    }

    namespace energy
    {
        enum energyenum
        {
            croccoBusemann   = 98304,
            ODE              = 70209
        };
    }
}

#endif
