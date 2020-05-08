#ifndef EQ_TYPES_H
#define EQ_TYPES_H
#include "HybridComputing.h"

#pragma HWPP include

namespace HyCore
{
    namespace equationType
    {
        enum equationTypeenum
        {
            explicitEq     = 83982,
            algebraicEq    = 20943,
            differentialEq = 79828
        };
    }

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
            ODE              = 23414,
            vanDriest        = 82984
        };
    }

    namespace energy
    {
        enum energyenum
        {
            croccoBusemann   = 98304,
            ODE              = 70209,
            linear           = 27498
        };
    }
}

#endif
